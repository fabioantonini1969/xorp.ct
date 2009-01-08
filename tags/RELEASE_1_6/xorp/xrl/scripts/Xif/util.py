import sys, string, re
from time import time, localtime, strftime

# -----------------------------------------------------------------------------
# Utility methods and structures

def quit(file, lineno, reason):
    print "In Xrl starting at line %d in %s:" % (lineno, file)
    print "\t", reason
    sys.exit(1)

def file_write_string(fname, data):
    f = open(fname, "w")
    ts = re.compile(r'[ \t]+$', re.MULTILINE)
    data = ts.sub("", data)
    f.write(data)
    f.close()

def file_append_string(fname, data):
    f = open(fname, "a")
    ts = re.compile(r'[ \t]+$', re.MULTILINE)
    data = ts.sub("", data)
    f.write(data)
    f.close()

def xorp_indent(level):
    if level <= 0:
        return ""
    s = ""
    ntabs = level / 2
    for i in range(0, ntabs):
        s += "\t"

    nspaces = level % 2
    if (nspaces):
        s += "    "
    return s

def xorp_indent_string(level, x):
    return xorp_indent(level) + x

def xorp_indent_list(level, list):
    r = []
    for l in list:
        r.append(xorp_indent_string(level, l));
    return r;

# Fix Xrl interface, method, and argument names for c++ 
def cpp_name(xrl_name):
    new_name = ""
    for i in range(0, len(xrl_name)):
        c = xrl_name[i]
        if (c == '/') | (c == '-') | (c == "."):
            c = "_"
        new_name += c
    return new_name

# Capitalize proper names
def proper(n):
    l = ' '
    r = ""
    for i in range(0, len(n)):
        if string.find(string.whitespace, l) >= 0:
            r += string.upper(n[i])
        else:
            r += string.lower(n[i])
        l = n[i]
    return r

# Generate class name
def cpp_classname(s):
    return proper(s.replace("-", " ").replace("_", " ")).replace(" ", "").replace(".", "_")

# Generate a capitalized class name "dilbert_chaser" -> "DilbertChaser"
def caps_cpp_classname(s):
    apply_cap = 1
    r = ""
    for l in s:
        digit = (string.find(string.digits, l) >= 0);
        if (digit):
            apply_cap = 1
        elif (string.find(string.letters, l) < 0):
            apply_cap = 1
            l = ''
        elif (apply_cap):
            l = string.upper(l)
            apply_cap = 0
        r += l
    return r

def cpp_version(v):
    # v looks like %d.%d
    return ("V" + v).replace(".", "p")

# Transform list of strings into a comma separated values string
def csv(l, comma = ", "):
    s = ''
    n = len(l)
    if (n >= 1):
        s = "%s" % l[0]
    for i in range(1,n):
        s += "%s%s" % (comma, l[i])
    return s;

# Transform list of strings into a comma separated values string suitable
# for appending to an existing comma separated value string.
def joining_csv(l, comma = ", "):
    s = ''
    n = len(l)
    for i in range(0,n):
        s += "%s%s" % (comma, l[i])
    return s;

# Extract $XORP line if exists
def get_xorp_cvs_id(filename):

    xorp_id = "$%s$" % "XORP"

    try:
        f = open(filename, "r")
    except:
        return xorp_id

    for line in f.readlines():
        mo = re.search("\$XORP.*\$", line)
        if mo != None:
            xorp_id = line[mo.start():mo.end()]
            break
    f.close()
    return xorp_id

# Generate standard header gubbins
def standard_preamble(is_header, filename):
    global g_source_files

    s = \
 """/*
 * Copyright (c) 2001-2009 XORP, Inc.
 * See the XORP LICENSE.lgpl file for licensing, conditions, and warranties
 * on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by '%s'.
""" % sys.argv[0][sys.argv[0].rfind("/") + 1:]

    xorp_cvs_id = get_xorp_cvs_id(filename)

    if is_header:
        s += " *\n * %s\n" % xorp_cvs_id

    s += " */\n\n"
    if is_header == 0:
        s += "#ident \"%s\"\n\n" % xorp_cvs_id

    return s;

def xrl_method_name(ifname, version, if_methodname):
    return "%s/%s/%s" % (ifname, version, if_methodname)
