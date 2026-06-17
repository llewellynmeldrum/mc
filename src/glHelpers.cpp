#include "glHelpers.hpp"
#include "Logger.hpp"
#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <regex>
size_t write_cb(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* out = static_cast<std::string*>(userdata);
    out->append(ptr, size * nmemb);
    return size * nmemb;
}

std::optional<std::string> fetch_url(const std::string& url, bool enable_logging=false) {
    auto start = get_current_us();
    CURL* curl = curl_easy_init();
    if (!curl) return std::nullopt;

    std::string body;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "gl-error-lookup/1.0");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    const CURLcode rc = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_cleanup(curl);

    if (rc != CURLE_OK || http_code != 200) return std::nullopt;
    auto end = get_current_us();
    if (enable_logging){
        std::println(stderr, "fetch took {:0.2f}ms",(end-start)/1000.0);
    }
    return body;
}

// Collapse runs of whitespace into single spaces and trim.
std::string normalize_ws(const std::string& in) {
    std::string out = std::regex_replace(in, std::regex(R"(\s+)"), " ");
    const auto b = out.find_first_not_of(' ');
    const auto e = out.find_last_not_of(' ');
    if (b == std::string::npos) return {};
    return out.substr(b, e - b + 1);
}


std::string lookup_gl_error_description( 
    const std::string& function_name,
    const std::string& error){
    const std::string url =
        "https://registry.khronos.org/OpenGL-Refpages/gl4/html/"
        + function_name + ".xhtml";

    const auto html = fetch_url(url,false);
    if (!html) return std::format("unable to fetch html. Try visiting: {}",url);

    htmlDocPtr doc = htmlReadMemory(
        html->c_str(), static_cast<int>(html->size()),
        url.c_str(), "UTF-8",
        HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

    if (!doc) return std::format("unable to parse doc. Try visiting: {}",url);

    xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
    if (!ctx){
        xmlFreeDoc(doc);
        return std::format("unable to create xml path context. Try visiting: {}",url);
    }

    const auto* expr = reinterpret_cast<const xmlChar*>(
            "//div[@id='errors']//p");

    xmlXPathObjectPtr res = xmlXPathEvalExpression(expr, ctx);

    std::vector<std::string> matching;
    std::vector<std::string> all_errors;

    if (res && res->nodesetval) {
        for (int i = 0; i < res->nodesetval->nodeNr; ++i) {
            xmlNodePtr node = res->nodesetval->nodeTab[i];
            xmlChar* content = xmlNodeGetContent(node);
            if (!content) continue;

            std::string text = normalize_ws(
                reinterpret_cast<const char*>(content));
            xmlFree(content);

            if (text.empty()) continue;
            all_errors.push_back(text);

            // Each error <p> names its GL_* token in the text, so a
            // substring match against the token isolates the relevant one.
            auto pos = text.find(error);
            if (pos != std::string::npos) {
                matching.push_back(
                    std::format("{} : {}",
                        fmt::styled(fmt::bold()+ fmt::fg_rgb(200,0,0),error),
                        fmt::styled(fmt::fg_rgb(200,0,0),text.substr(pos+error.length()))
                    )
                );
                
            }
        }
    }

    if (res){
        xmlXPathFreeObject(res);
    }
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);

    if (!matching.empty()) {
        // A single error token can appear in multiple conditions
        // (e.g. several ways to trigger GL_INVALID_OPERATION); join them.
        std::ostringstream oss;
        for (size_t i = 0; i < matching.size(); ++i) {
            if (i) oss << '\n';
            oss << matching[i];
        }
        oss << "\n\n(fetched from Khronos opengl ref-pages: " << 
            fmt::styled(fmt::fg_rgb(30,90,255),url) << ")\n";
        return oss.str();
    }

    // Token not found, but the page had an errors section: fall back to
    // returning everything so the caller still gets context.
    if (!all_errors.empty()) {
        std::ostringstream oss;
        for (size_t i = 0; i < all_errors.size(); ++i) {
            if (i) oss << '\n';
            oss << all_errors[i];
        }
        oss << "\n\n(fetched from Khronos opengl ref-pages: " << 
            fmt::styled(fmt::fg_rgb(30,90,200),url) << ")\n";
        return oss.str();
    }

    return "???";
}
