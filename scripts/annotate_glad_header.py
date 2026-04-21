import xml.etree.ElementTree as ET
from dataclasses import dataclass
import sys
from typing import List
from typing import Dict

if len(sys.argv) < 2:
    DEBUG = False
elif sys.argv[1] == "-d":
    DEBUG = True
else:
    DEBUG = False

END = ""


@dataclass
class Param:
    qualifiers: str
    T: str
    name: str
    ptr: str

    def __str__(self):
        return f"{self.qualifiers}{self.T}{self.ptr}{self.name}"


class Command:
    comments: str
    proto: ET.Element
    return_t: str
    macro_name: str
    function_name: str
    params: list[Param]

    def __init__(self, cmd_obj):
        self.cmd_obj = cmd_obj
        self.proto = cmd_obj.find("proto")
        self.macro_name = "NONE"
        self.return_t = "NONE"
        self.comments = ""
        self.params = []

    def __str__(self):
        fmt = ""
        if "glGetStringi" in self.macro_name or "glGetString" in self.macro_name:
            self.return_t = "const GLubyte*"
        fmt += f"{self.return_t} {self.macro_name}("
        for param in self.params:
            fmt += str(param)
            if param is not self.params[-1]:
                fmt += ", "
        fmt += ")"
        return fmt

    def parseFunctionName(self):
        if self.proto.find("name") is None:
            return False
        self.macro_name = self.proto.find("name").text
        return True

    def parseReturnType(self):
        if self.proto.text is not None:
            self.return_t = self.proto.text
            return True
        elif self.proto.find("ptype") is not None:
            self.return_t = self.proto.find("ptype").text
            return True
        return False

    def parseParams(self):
        param_objs = self.cmd_obj.findall("param")
        for param_obj in param_objs:
            qualifiers = ""
            param_name = ""
            param_t = ""
            tail = ""

            if param_obj.find("name") is None:
                return False
            else:
                param_name = param_obj.find("name").text

            if param_obj.find("ptype") is not None:
                param_t = param_obj.find("ptype").text
                if param_obj.find("ptype").tail is not None:
                    tail = param_obj.find("ptype").tail
                if param_obj.text is not None:
                    qualifiers = param_obj.text
            elif param_obj.text is not None:
                param_t = param_obj.text
            else:
                return False

            self.comments += f"//  @param {param_name}"
            if param_obj.get("kind", default="") in kinds:
                self.comments += f" {kinds[param_obj.get('kind')][26:-1]}"
            if param_obj.get("group") is not None:
                self.comments += f", *{param_obj.get('group', default='')}*"
            if param_obj.get("len") is not None:
                self.comments += f", len: {param_obj.get('len', default='')}"

            self.comments += "\n"
            self.params.append(Param(qualifiers, param_t, param_name, tail))
        return True


count = 0
badcount = 0


def error(msg):
    global badcount
    badcount += 1
    print(f"[{count}]Error: {msg}")


def parseCommands(file):
    global count
    tree = ET.parse(file)

    # get root element
    root = tree.getroot()
    parsed_commands = []
    commands = root.find("commands")
    for cmd_obj in commands:
        count += 1
        cmd = Command(cmd_obj)
        if cmd.parseFunctionName() is False:
            error("unable to parse function name")
            continue
        if cmd.parseReturnType() is False:
            error(f"unable to parse return type for {cmd.macro_name}")
            continue

        if cmd.parseParams() is False:
            error(f"unable to parse return params for {cmd.macro_name}")
            continue

        parsed_commands.append(cmd)
    # return news items list
    return parsed_commands


def get_true_function_name(cmd: Command):
    function_name = "glad_"
    if DEBUG:
        function_name += "debug_"
    function_name += cmd.macro_name
    return function_name


def true_function_call(cmd: Command):
    fmt = ""
    fmt += f"    {cmd.function_name}("
    for param in cmd.params:
        fmt += f"{param.name}"
        if param is not cmd.params[-1]:
            fmt += ", "
    fmt += ")"
    return fmt


def print_remapped_signature(cmd: Command):
    cmd.function_name = get_true_function_name(cmd)
    res = ""
    res = f"#ifdef {cmd.macro_name}\n"
    res += f"#undef {cmd.macro_name}\n"
    res += "\n"
    res += f"{cmd.comments}"
    res += f"\tinline __attribute__((always_inline)) {cmd}"
    res += "{\n\t"
    if cmd.return_t != "void":
        res += "\treturn "

    res += true_function_call(cmd) + ";"
    res += "\n\t}"
    res += f"\n#endif // {cmd.macro_name}\n"
    print(res)


def parseKinds(file):
    tree = ET.parse(file)

    root = tree.getroot()
    kind_objs = root.find("kinds")
    kinds = {}
    for kind_obj in kind_objs:
        kinds[kind_obj.get("name")] = kind_obj.get("desc")
    return kinds


kinds: Dict[str, str] = parseKinds("./resources/gl.xml")
wrapperCommands: List[Command] = parseCommands("./resources/gl.xml")
# for key, value in kinds.items():
#    print(f"{key}: {value}")
for cmd in wrapperCommands:
    print_remapped_signature(cmd)
