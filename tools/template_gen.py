import argparse
import datetime

parser = argparse.ArgumentParser(description='Generate the modules C and H file templates')
parser.add_argument('-m', '--module', help='Module name. Like i2c etc.', required=True)
parser.add_argument('-d', '--description', help='What is the module about.', required=True)
parser.add_argument('-a', '--author', help='Your name as author displayed on top of module', required=True)

args = parser.parse_args()

module_name = args.module.lower()
modules_name_upper = module_name.upper()
module_description = args.description
author = args.author
current_year = datetime.datetime.now().year
eol_char = "\n"

c_file_template = \
    "/*! \\file {module_name}.c{eol}"\
    "*{eol}"\
    "*  \\brief {module_description}.{eol}"\
    "*{eol}"\
    "*{eol}"\
    "*  \\author {author}{eol}"\
    "*/{eol}"\
    "{eol}"\
    "{eol}"\
    "/********************************** Includes *******************************************/{eol}"\
    "#include \"{module_name}.h\"{eol}"\
    "{eol}"\
    "/*********************************** Consts ********************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/************************************ Types ********************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/*********************************** Macros ********************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/******************************* Global Variables **************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/******************************** Local Variables **************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/****************************** Functions Prototype ************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/****************************** Functions Definition ***********************************/{eol}"\
    "/**{eol}"\
    "* \\name     {modules_name_upper}_init{eol}"\
    "* \\brief    Initialize the module variables and resources.{eol}"\
    "*{eol}"\
    "* \\param    None{eol}"\
    "* \\retval   None{eol}"\
    "*/{eol}"\
    "void {modules_name_upper}_init( void ){eol}"\
    "{{{eol}" \
    "   /* Initialize the module here*/{eol}"\
    "{eol}"\
    "}}{eol}".format(eol=eol_char, module_name=module_name, modules_name_upper=modules_name_upper, module_description=module_description, author=author, year=current_year)


h_file_template = \
    "/*! \\file {module_name}.h{eol}"\
    "*{eol}"\
    "*  \\brief {module_name} module functions and variables declarations.{eol}"\
    "*{eol}"\
    "*{eol}"\
    "*  \\author {author}{eol}"\
    "*/{eol}"\
    "{eol}"\
    "#pragma once{eol}"\    
    "{eol}"\
    "#ifdef __cplusplus{eol}"\
    "extern \"C\" {{{eol}"\
    "#endif{eol}"\
    "/********************************** Includes *******************************************/{eol}"\
    "#include \"common.h\"{eol}"\
    "{eol}"\
    "/*********************************** Consts ********************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/************************************ Types ********************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/*********************************** Macros ********************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/******************************* Global Variables **************************************/{eol}"\
    "{eol}"\
    "{eol}"\
    "/****************************** Functions Prototype ************************************/{eol}"\
    "void {modules_name_upper}_init( void );{eol}"\
    "{eol}"\
    "{eol}"\
    "#ifdef __cplusplus{eol}"\
    "}}{eol}"\
    "#endif{eol}"\
    "{eol}"\
    "#endif /* __{modules_name_upper}_H__ */{eol}".format(eol=eol_char, module_name=module_name, modules_name_upper=modules_name_upper, module_description=module_description, author=author, year=current_year)

c_file = open("{}.c".format(module_name),"w+")
c_file.write(c_file_template)
c_file.close()


h_file = open("{}.h".format(module_name),"w+")
h_file.write(h_file_template)
h_file.close()
