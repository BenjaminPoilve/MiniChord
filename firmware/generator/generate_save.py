import json
from airium import Airium
from itertools import groupby

a = Airium(
    base_indent='  ',  # str
    current_level=0,  # int
    source_minify=False,  # bool
    source_line_break_character="\n",  # str
    )

cpp_start_file="void apply_audio_parameter(int adress, int value) {\r\n    switch(adress){\r\n"
cpp_end_file="  }\r\n}"
id_iterator=0

#Making the HTML file
with a.html():
    with a.head():
        a.link(href='index.css', rel='stylesheet')

with open('parameters.json') as f: # Reserved adresses: 0 for system command and 1 for bank adress
    d = json.load(f)
    with a.html():
        with a.body(id="body", klass="control_full"):
            with a.div(id="front_page"):
                with a.div(id='header',klass="line"):
                    with a.div(klass="title bloc B6 M6 S6"):
                        a.h1(_t='• Minichord')
                    with a.div(klass="bloc B3 M3 S3"):
                        a.p(_t='Current bank : unconnected', id="bank_number_zone")
                    with a.div(klass="bloc B3 M3 S3"):
                        with a.div(klass="line"):
                            with a.div(klass="bloc B3 M3 S3"):
                                with a.select(id='bank_number_selection', name='bank number'):
                                    for i in range(12):
                                        a.option(value=i, _t=(i+1))
                            with a.div(klass="bloc B3 M3 S3"):
                                a.button(onclick='save_current_settings()', _t='Save current settings to selected bank',klass="inactive")
                    with a.div(klass="bloc B3 M3 S3"):
                        a.button(onclick='generate_settings()', _t='Share settings',klass="inactive")
                    with a.div(klass="bloc B3 M3 S3"):
                        a.button(onclick='load_settings()', _t='Load settings',klass="inactive")
                    with a.div(klass="bloc B3 M3 S3"):
                        a.button(onclick='reset_selected_bank()', _t='Reset selected bank',klass="inactive")
                    with a.div(klass="bloc B3 M3 S3"):
                        a.button(onclick='reset_memory()', _t='Reset all memory',klass="inactive")
                with a.div(tabindex='1', id='information_zone',klass="line"):
                     a.p(_t='Please connect the Minichord then reload the page to continue', id='information_text')
                with a.div( id='instruction_zone',klass="line"):
                    a.p(_t='Here we have the specific instruction for connecting')
            if "global_parameter" in d:
                with a.div(id='header',klass="line parameter_section"):
                    with a.div(klass="bloc B3 M9 S9"):
                        a.h4(_t='Global parameters', klass="inactive")
                    #d["harp_parameter"].sort(key= lambda content: content["group"])
                    grouped=groupby(d["global_parameter"],lambda content: content["group"])
                    with a.div(klass="bloc B8 M9 S9"):
                        with a.div(klass="line"):
                            for name, parameter_group in grouped:
                                with a.div(klass="inactive bloc B4 M4 S4 parameter_group"):
                                    a.h4(_t=name)
                                    for parameter in parameter_group:
                                        try:
                                            print(parameter)
                                            with a.div(name=id_iterator,klass="line"):
                                                with a.div(klass=" bloc B8 M6 S6"):
                                                    a.dfn(title=parameter["tooltip"], _t=parameter["name"])
                                                    #a.label(for_=id_iterator, _t=parameter["name"])
                                                with a.div(klass=" bloc B3 M4 S4"):
                                                    if(parameter["data_type"]=="int"):
                                                        a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='1', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                                    else:
                                                        a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='0.01', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                                with a.div(klass=" bloc B2 M4 S4"):
                                                    a.p(id="value_zone"+str(parameter["sysex_adress"]),klass='value_zone')
                                                id_iterator+=1
                                        except KeyError:
                                            print("Missing entry parameter in the JSON item : ")
                                            print(parameter)
                                            break
            else:
                print("Missing global parameters in JSON")
            if "harp_parameter" in d:
                with a.div(id='header',klass="line parameter_section"):
                    with a.div(klass="bloc B3 M9 S9"):
                        a.h4(_t='Harp parameters', klass="inactive")
                    #d["harp_parameter"].sort(key= lambda content: content["group"])
                    grouped=groupby(d["harp_parameter"],lambda content: content["group"])
                    with a.div(klass="bloc B8 M9 S9"):
                        with a.div(klass="line"):
                            grouped=groupby(d["harp_parameter"],lambda content: content["group"])
                            for name, parameter_group in grouped:
                                with a.div(klass="inactive bloc B4 M4 S4 parameter_group"):
                                    a.h4(_t=name)
                                    for parameter in parameter_group:
                                        try:
                                            print(parameter)
                                            with a.div(name=id_iterator,klass="line"):
                                                with a.div(klass=" bloc B8 M6 S6"):
                                                    a.dfn(title=parameter["tooltip"], _t=parameter["name"])
                                                with a.div(klass=" bloc B4 M4 S4"):
                                                    if(parameter["data_type"]=="int"):
                                                        a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='1', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                                    else:
                                                        a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='0.01', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                                with a.div(klass=" bloc B2 M4 S4"):
                                                    a.p(id="value_zone"+str(parameter["sysex_adress"]),klass='value_zone')
                                                id_iterator+=1
                                        except KeyError:
                                            print("Missing entry parameter in the JSON item : ")
                                            print(parameter)
                                            break
            else:
                print("Missing harp parameters in JSON")
            if "chord_parameter" in d:
                with a.div(id='header',klass="line parameter_section"):
                    with a.div(klass="bloc B3 M9 S9"):
                        a.h4(_t='Chord parameters', klass="inactive")
                    #d["harp_parameter"].sort(key= lambda content: content["group"])
                    grouped=groupby(d["chord_parameter"],lambda content: content["group"])
                    with a.div(klass="bloc B8 M9 S9"):
                        with a.div(klass="line"):
                            for name, parameter_group in grouped:
                                with a.div(klass="inactive bloc B4 M4 S4 parameter_group"):
                                    a.h4(_t=name)
                                    for parameter in parameter_group:
                                        try:
                                            print(parameter)
                                            with a.div(name=id_iterator,klass="line",id=parameter["sysex_adress"]):
                                                with a.div(klass=" bloc B8 M6 S6"):
                                                    a.dfn(title=parameter["tooltip"], _t=parameter["name"])
                                                with a.div(klass=" bloc B4 M4 S4"):
                                                    if(parameter["data_type"]=="int"):
                                                        a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='1', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                                    else:
                                                        a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='0.01', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                                with a.div(klass=" bloc B2 M4 S4"):
                                                    a.p(id="value_zone"+str(parameter["sysex_adress"]),klass='value_zone')
                                                id_iterator+=1
                                        except KeyError:
                                            print("Missing entry parameter in the JSON item : ")
                                            print(parameter)
                                            break
            else:
                print("Missing Chord parameters in JSON")
            a.script(src='index.js')

            a.p(_t='Output:')
            a.p(id="output_zone")

Html_file= open("../controller/index.html","w")
Html_file.write(str(a))
Html_file.close()

#Making the CPP file


with open('parameters.json') as f:
    d = json.load(f)
    with open('../include/sysex_handler.h', 'w') as cpp_output:
        cpp_output.write(cpp_start_file)
        if "global_parameter" in d:
            try:
                for parameter in d["global_parameter"]:
                    method=parameter["method"]
                    if(parameter["data_type"]=="float"):
                        method=method.replace("value","value/100.0")
                    cpp_output.write("      case "+str(parameter["sysex_adress"])+":\r\n")
                    if(parameter["iterate"]>1):
                        cpp_output.write("        for (int i=0;i<"+str(parameter["iterate"])+";i++){\r\n")
                        cpp_output.write("          "+method+"\r\n")
                        cpp_output.write("        }\r\n")
                    else:
                        cpp_output.write("        "+method+"\r\n")
                    cpp_output.write("        break;\r\n")

            except KeyError:
                print("Missing entry parameter in the JSON item : ")
                print(parameter)
        else:
            print("Missing global parameters in JSON")
        if "harp_parameter" in d:
            try:
                for parameter in d["harp_parameter"]:
                    method=parameter["method"]
                    if(parameter["data_type"]=="float"):
                        method=method.replace("value","value/100.0")
                    cpp_output.write("      case "+str(parameter["sysex_adress"])+":\r\n")
                    if(parameter["iterate"]>1):
                        cpp_output.write("        for (int i=0;i<"+str(parameter["iterate"])+";i++){\r\n")
                        cpp_output.write("          "+method+"\r\n")
                        cpp_output.write("        }\r\n")
                    else:
                        cpp_output.write("        "+method+"\r\n")
                    cpp_output.write("        break;\r\n")

            except KeyError:
                print("Missing entry parameter in the JSON item : ")
                print(parameter)
        else:
            print("Missing harp parameters in JSON")
        if "chord_parameter" in d:
            try:
                for parameter in d["chord_parameter"]:
                    method=parameter["method"]
                    if(parameter["data_type"]=="float"):
                        method=method.replace("value","value/100.0")
                    cpp_output.write("      case "+str(parameter["sysex_adress"])+":\r\n")
                    if(parameter["iterate"]>1):
                        cpp_output.write("        for (int i=0;i<"+str(parameter["iterate"])+";i++){\r\n")
                        cpp_output.write("          "+method+"\r\n")
                        cpp_output.write("        }\r\n")
                    else:
                        cpp_output.write("        "+method+"\r\n")
                    cpp_output.write("        break;\r\n")

            except KeyError:
                print("Missing entry parameter in the JSON item : ")
                print(parameter)
        else:
            print("Missing chord parameters in JSON")
        cpp_output.write(cpp_end_file)