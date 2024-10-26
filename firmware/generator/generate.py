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
        with a.head():
            a.meta(charset='UTF-8')
            a.meta(content='width=device-width, initial-scale=1.0', name='viewport')
            a.meta(content='ie=edge', **{'http-equiv': 'X-UA-Compatible'})
            a.link(href='../img/android-chrome-192x192.png', rel='icon', sizes='192x192', type='image/png')
            a.link(href='../img/android-chrome-512x512.png', rel='icon', sizes='512x512', type='image/png')
            a.link(href='../img/apple-touch-icon.png', rel='apple-touch-icon', sizes='180x180')
            a.link(href='../img/favicon.ico', rel='shortcut icon', sizes='48x48', type='image/png')
            a.link(href='../img/favicon-16x16.png', rel='icon', sizes='16x16', type='image/png')
            a.link(href='../img/favicon-32x32.png', rel='icon', sizes='32x32', type='image/png')
            a.title(_t='minicontrol')
        with a.body(id="body", klass="control_full"):
            with a.div(id='content',klass="line"):
                with a.div(id="control", klass="bloc B3 M4 S9"):
                    with a.div(id='header',klass="line"):
                        with a.div(klass="title bloc B9 M9 S9"):
                            a.h4(_t='minicontrol')
                        with a.div(id='status_zone', klass="unconnected"):
                            a.span(_t='●',id='dot')
                            a.span(_t='',id='status_value')
                      
                        with a.div(klass="line"):
                            a.h5(_t='saving:',klass="inactive")
                        with a.div(klass="line"):
                            with a.div(klass="bloc B3 M3 S3 button_div"):
                                with a.div( klass="select_container"):
                                    a.span(_t='target bank:', klass="inactive")
                                    with a.select(id='bank_number_selection', klass="inactive", name='bank number'):
                                        for i in range(12):
                                            a.option(value=i, _t=(i+1), klass="inactive")
                            with a.div(klass="bloc B3 M3 S3 button_div"):
                                a.button(onclick='save_current_settings()', _t='save to bank',klass="inactive")
                        with a.div(klass="line"):
                            a.h5(_t='sharing:',klass="inactive")
                        with a.div(klass="line"):
                            with a.div(klass="bloc B3 M3 S3 button_div"):
                                a.button(onclick='generate_settings()', _t='export settings',klass="inactive")
                            with a.div(klass="bloc B3 M3 S3 button_div"):
                                a.button(onclick='load_settings()', _t='load settings',klass="inactive")
                        with a.div(klass="line"):
                            a.h5(_t='resetting:',klass="inactive")
                        with a.div(klass="line"):
                            with a.div(klass="bloc B3 M3 S3 button_div"):
                                a.button(onclick='reset_current_bank()', _t='reset bank',klass="inactive")
                            with a.div(klass="bloc B3 M3 S3 button_div"):
                                a.button(onclick='reset_memory()', _t='reset all banks',klass="inactive")
                    with a.details():
                        with a.summary():
                            a.b(_t='Connection instruction')
                        with a.ul(klass="instruction_steps"):
                            with a.li(id="step1", klass="unsatisfied"):
                                a('provide the system autorisation for MIDI control')
                            with a.li(id="step2", klass="unsatisfied"):
                                a('use a recent version of Chrome')
                            with a.li(id="step3", klass="unsatisfied"):
                                a('connect the minichord with a USB cable and make sure it is on.')
                        with a.div(tabindex='1', id='information_zone'):
                            a.strong(_t='> Please follow the steps above',id='information_text')
                    
                    with a.div( id='instruction_zone'):
                        a('For instruction on how to use this tool, please refer to the ')
                        a.a(href='../user_manual/#custom-presets', _t='minichord documentation.')
                a.div(id="spacer", klass="bloc B0 M0 S0")
                with a.div(id="parameters", klass="bloc B8 M8 S9"):
                    with a.div(klass="array_content"):
                        with a.div(name=id_iterator,klass="line header_data data_line inactive"):
                            with a.div(klass=" bloc B4 M9 S9"):
                                a.p(_t="Global parameters" ,klass="row_title")
                            with a.div(klass=" bloc B3 M2 S3"):
                                a.p(_t="Subcategory")
                            with a.div(klass=" bloc B1 M1 S1"):
                                a.p(_t="ID")
                            with a.div(klass=" bloc B6 M4 S6"):
                                a.p(_t="Name")
                            with a.div(klass=" bloc B2 M2 S4"):
                                a.p(_t="Control")
                            with a.div(klass=" bloc B1 M1 S1 right_aligned"):
                                a.p(_t="Value")
                        with a.div(name=id_iterator,klass="line data_line"):
                            a.hr()
                        if "global_parameter" in d:
                            grouped=groupby(d["global_parameter"],lambda content: content["group"])
                            global_name_written =False
                            for name, parameter_group in grouped:
                                sub_name_written =False
                                for parameter in parameter_group:
                                    try:
                                        class_group="line data_line content_line inactive"
                                        if(name=="hidden"):
                                            class_group+=" hidden"
                                        with a.div(name=id_iterator,klass=class_group):
                                            with a.div(klass=" bloc B4 M0 S0"):
                                                if(not global_name_written):
                                                    a.p(_t="")
                                                    global_name_written=True
                                                else:
                                                    a.p(_t="")
                                            with a.div(klass=" bloc B3 M2 S3"):
                                                if(not sub_name_written):
                                                    a.p(_t=name)
                                                    sub_name_written=True
                                                else:
                                                    a.p(_t="")
                                            with a.div(klass=" bloc B1 M1 S1"):
                                                a.p(_t=parameter["sysex_adress"])
                                            with a.div(klass=" bloc B6 M4 S6"):
                                                a.dfn(title=parameter["tooltip"], _t=parameter["name"])
                                                #a.label(for_=id_iterator, _t=parameter["name"])
                                            with a.div(klass=" bloc B2 M2 S4"):
                                                if(parameter["data_type"]=="int"):
                                                    a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='1', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                                else:
                                                    a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='0.01', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                            with a.div(klass=" bloc B1 M1 S1"):
                                                a.p(id="value_zone"+str(parameter["sysex_adress"]),klass='value_zone')
                                            id_iterator+=1
                                    except KeyError:
                                        print("Missing entry parameter in the JSON item : ")
                                        print(parameter)
                                        break
                        else:
                            print("Missing global parameters in JSON")
                    with a.div(klass="array_content"):
                        with a.div(name=id_iterator,klass="line header_data data_line inactive"):
                            with a.div(klass=" bloc B4 M9 S9"):
                                a.p(_t="Harp parameters" ,klass="row_title")
                            with a.div(klass=" bloc B3 M2 S3"):
                                a.p(_t="Subcategory")
                            with a.div(klass=" bloc B1 M1 S1"):
                                a.p(_t="ID")
                            with a.div(klass=" bloc B6 M4 S6"):
                                a.p(_t="Name")
                            with a.div(klass=" bloc B2 M2 S4"):
                                a.p(_t="Control")
                            with a.div(klass=" bloc B1 M1 S1 right_aligned"):
                                a.p(_t="Value")
                        with a.div(name=id_iterator,klass="line data_line"):
                            a.hr()
                        if "harp_parameter" in d:
                            grouped=groupby(d["harp_parameter"],lambda content: content["group"])
                            global_name_written =False
                            for name, parameter_group in grouped:
                                sub_name_written =False
                                for parameter in parameter_group:
                                    try:
                                        class_group="line data_line content_line inactive"
                                        if(name=="hidden"):
                                            class_group+=" hidden"
                                        with a.div(name=id_iterator,klass=class_group):
                                            with a.div(klass=" bloc B4 M0 S0"):
                                                if(not global_name_written):
                                                    a.p(_t="")
                                                    global_name_written=True
                                                else:
                                                    a.p(_t="")
                                            with a.div(klass=" bloc B3 M2 S3"):
                                                if(not sub_name_written):
                                                    a.p(_t=name)
                                                    sub_name_written=True
                                                else:
                                                    a.p(_t="")
                                            with a.div(klass=" bloc B1 M1 S1"):
                                                a.p(_t=parameter["sysex_adress"])
                                            with a.div(klass=" bloc B6 M4 S6"):
                                                a.dfn(title=parameter["tooltip"], _t=parameter["name"])
                                                #a.label(for_=id_iterator, _t=parameter["name"])
                                            with a.div(klass=" bloc B2 M2 S4"):
                                                if(parameter["data_type"]=="int"):
                                                    a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='1', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                                else:
                                                    a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='0.01', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                            with a.div(klass=" bloc B1 M1 S1"):
                                                a.p(id="value_zone"+str(parameter["sysex_adress"]),klass='value_zone')
                                            id_iterator+=1
                                    except KeyError:
                                        print("Missing entry parameter in the JSON item : ")
                                        print(parameter)
                                        break
                        else:
                            print("Missing harp parameters in JSON")
                    with a.div(klass="array_content"):
                        with a.div(name=id_iterator,klass="line header_data data_line inactive"):
                            with a.div(klass=" bloc B4 M9 S9"):
                                a.p(_t="Chord parameters",klass="row_title")
                            with a.div(klass=" bloc B3 M2 S3"):
                                a.p(_t="Subcategory")
                            with a.div(klass=" bloc B1 M1 S1"):
                                a.p(_t="ID")
                            with a.div(klass=" bloc B6 M4 S6"):
                                a.p(_t="Name")
                            with a.div(klass=" bloc B2 M2 S4"):
                                a.p(_t="Control")
                            with a.div(klass=" bloc B1 M1 S1 right_aligned"):
                                a.p(_t="Value")
                        with a.div(name=id_iterator,klass="line data_line"):
                            a.hr()
                        if "chord_parameter" in d:
                            grouped=groupby(d["chord_parameter"],lambda content: content["group"])
                            global_name_written =False
                            for name, parameter_group in grouped:
                                sub_name_written =False
                                for parameter in parameter_group:
                                    try:
                                        class_group="line data_line content_line inactive"
                                        if(name=="hidden"):
                                            class_group+=" hidden"
                                        with a.div(name=id_iterator,klass=class_group, id=parameter["sysex_adress"]):
                                            with a.div(klass=" bloc B4 M0 S0"):
                                                if(not global_name_written):
                                                    a.p(_t="")
                                                    global_name_written=True
                                                else:
                                                    a.p(_t="")
                                            with a.div(klass=" bloc B3 M2 S3"):
                                                if(not sub_name_written):
                                                    a.p(_t=name)
                                                    sub_name_written=True
                                                else:
                                                    a.p(_t="")
                                            with a.div(klass=" bloc B1 M1 S1"):
                                                a.p(_t=parameter["sysex_adress"])
                                            with a.div(klass=" bloc B6 M4 S6"):
                                                a.dfn(title=parameter["tooltip"], _t=parameter["name"])
                                                #a.label(for_=id_iterator, _t=parameter["name"])
                                            with a.div(klass=" bloc B2 M2 S4"):
                                                if(parameter["data_type"]=="int"):
                                                    a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='1', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                                else:
                                                    a.input(klass="slider inactive",adress_field=parameter["sysex_adress"], curve=parameter["curve"],data_type=parameter["data_type"], id=id_iterator, max=parameter["max_value"], min=parameter["min_value"], onchange='handlechange(this)', step='0.01', target_max=parameter["max_value"], target_min=parameter["min_value"], type='range', value=parameter["default_value"])
                                            with a.div(klass=" bloc B1 M1 S1"):
                                                a.p(id="value_zone"+str(parameter["sysex_adress"]),klass='value_zone')
                                            id_iterator+=1
                                    except KeyError:
                                        print("Missing entry parameter in the JSON item : ")
                                        print(parameter)
                                        break
                        else:
                            print("Missing chord parameters in JSON")
                        
            a.script(src='index.js')
            a.p(id="output_zone")

Html_file= open("../minicontrol/index.html","w")
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