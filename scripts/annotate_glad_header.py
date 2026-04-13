import xml.etree.ElementTree as ET
from dataclasses import dataclass
from typing import List
from copy import copy

DEBUG = True
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
        if self.proto.text is None:
            return False
        self.return_t = self.proto.text or "void "
        self.return_t = self.return_t.rstrip(" ")
        return True

    def parseParams(self):
        param_objs = self.cmd_obj.findall("param")
        for param_obj in param_objs:
            if param_obj.find("name") is None:
                return False
            if param_obj.find("ptype") is None:
                return False

            qualifiers = ""
            if param_obj.text == "const ":
                qualifiers = param_obj.text

            param_name = param_obj.find("name").text
            param_t = param_obj.find("ptype").text
            tail = ""
            if param_obj.find("ptype").tail is not None:
                tail = param_obj.find("ptype").tail

            self.params.append(Param(qualifiers, param_t, param_name, tail))
        return True


def parseCommands(file):
    tree = ET.parse(file)

    # get root element
    root = tree.getroot()
    parsed_commands = []
    commands = root.find("commands")
    commands.findall("command")
    for cmd_obj in commands:
        cmd = Command(cmd_obj)
        if cmd.parseFunctionName() is False:
            continue
        if cmd.parseReturnType() is False:
            continue

        if cmd.parseParams() is False:
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
    FN_PROLOGUE = f"#ifdef {cmd.macro_name}\n"
    FN_PROLOGUE += f"#undef {cmd.macro_name}\n\tstatic inline "

    res = ""
    res += f"{FN_PROLOGUE}{cmd}"
    res += "{\n\t"
    if cmd.return_t != "void":
        res += "\treturn "

    res += true_function_call(cmd) + ";"
    res += "\n\t}"
    res += f"\n#endif // {cmd.macro_name}\n"
    print(res)


wrapperCommands: List[Command] = parseCommands("../resources/gl.xml")
for cmd in wrapperCommands:
    print_remapped_signature(cmd)
