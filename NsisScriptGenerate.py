import os
import sys

g_extract_file_cmd_total = 0
g_create_dir_cmd_total = 0
g_insert_nsis_script_install_list = []
g_insert_nsis_script_uninstall_list = []
g_root_dir = ""
g_root_dir_size = 0

def nsis_cmd_num_statistics(dir):
    global g_extract_file_cmd_total
    global g_create_dir_cmd_total
    if not os.path.isdir(dir):
        return

    files = os.listdir(dir)
    for fi in files:
        fi_d = os.path.join(dir, fi)
        if os.path.isdir(fi_d):
            g_create_dir_cmd_total += 1
            nsis_cmd_num_statistics(fi_d)
        else:
            g_extract_file_cmd_total += 1


def generate_nsis_script_install(dir):
    global g_root_dir_size
    cur_num = 0
    if not os.path.isdir(dir):
        return

    for root, dirs, files in os.walk(dir):

        # root 表示当前正在访问的文件夹路径
        # dirs 表示该文件夹下的子目录名list
        # files 表示该文件夹下的文件list

        # 遍历所有的文件夹
        for d in dirs:
            full_path = os.path.join(root, d)
            sub_path = full_path[len(g_root_dir):]
            cur_num += 1
            g_insert_nsis_script_install_list.append('    CreateDirectory "$INSTDIR{0}"\n'.format(sub_path))
            g_insert_nsis_script_install_list.append('    {0}::SetInstallStepDescription /NOUNLOAD "{1}" {2:.2f}\n'
                                             .format('${UI_PLUGIN_NAME}', sub_path[1:],
                                                     cur_num * 100 / (
                                                                 g_extract_file_cmd_total + g_create_dir_cmd_total)))
        # 遍历文件
        for f in files:
            full_path = os.path.join(root, f)
            sub_path = full_path[len(g_root_dir):]
            sub_dir = root[len(g_root_dir):]
            g_root_dir_size += os.path.getsize(full_path)
            cur_num += 1
            g_insert_nsis_script_install_list.append('    SetOutPath "$INSTDIR{0}"\n'.format(sub_dir))
            g_insert_nsis_script_install_list.append('    File "{0}"\n'.format(full_path))
            g_insert_nsis_script_install_list.append('    {0}::SetInstallStepDescription /NOUNLOAD "{1}" {2:.2f}\n'
                                             .format('${UI_PLUGIN_NAME}', sub_path[1:],
                                                     cur_num * 100 / (g_extract_file_cmd_total + g_create_dir_cmd_total)))


def generate_nsis_script_uninstall(dir):
    cur_num = 0
    if not os.path.isdir(dir):
        return

    for root, dirs, files in os.walk(dir):

        # root 当前正在访问的文件夹路径
        # dirs 该文件夹下的子目录名list
        # files 该文件夹下的文件list

        # 遍历文件
        for f in files:
            full_path = os.path.join(root, f)
            sub_path = full_path[len(g_root_dir):]
            cur_num += 1
            g_insert_nsis_script_uninstall_list.insert(0, '    Delete "$INSTDIR{0}"\n'.format(sub_path))
            g_insert_nsis_script_uninstall_list.insert(1, '    {0}::UnSetUnInstallStepDescription /NOUNLOAD "{1}" {2:.2f}\n'
                                             .format('${UI_PLUGIN_NAME}', sub_path[1:],
                                                     100 - (cur_num - 1) * 100 / (g_extract_file_cmd_total + g_create_dir_cmd_total)))
        # 遍历所有的文件夹
        for d in dirs:
            full_path = os.path.join(root, d)
            sub_path = full_path[len(g_root_dir):]
            cur_num += 1
            g_insert_nsis_script_uninstall_list.insert(0, '    RMDir "$INSTDIR{0}"\n'.format(sub_path))
            g_insert_nsis_script_uninstall_list.insert(1, '    {0}::UnSetUnInstallStepDescription /NOUNLOAD "{1}" {2:.2f}\n'
                                             .format('${UI_PLUGIN_NAME}', sub_path[1:],
                                                     100 - (cur_num - 1) * 100 / (
                                                                 g_extract_file_cmd_total + g_create_dir_cmd_total)))



def do_main(nsis_script_template_path):
    global g_root_dir
    global g_root_dir_size
    if g_root_dir[len(g_root_dir) - 1] == '\\':
        g_root_dir = g_root_dir[:len(g_root_dir) - 1]
    nsis_cmd_num_statistics(g_root_dir)
    print("file total: " + str(g_extract_file_cmd_total) + ", dir total: " + str(g_create_dir_cmd_total))

    generate_nsis_script_install(g_root_dir)
    g_insert_nsis_script_install_list.append('    ${UI_PLUGIN_NAME}::SetInstallStepDescription /NOUNLOAD "Completed" 100')

    generate_nsis_script_uninstall(g_root_dir)
    g_insert_nsis_script_uninstall_list.append('    ${UI_PLUGIN_NAME}::UnSetUnInstallStepDescription /NOUNLOAD "Completed" 100')

    f = open(nsis_script_template_path, 'r', encoding='utf-8')
    all_nsis_script_lines = []
    cur_line_index = -1
    insert_install_line_index = -1
    insert_uninstall_line_index = -1
    for s in f.readlines():
        all_nsis_script_lines.append(s)
        cur_line_index += 1
        if s.find('# onExtractFiles NsisScriptGenerate.py') != -1:
            insert_install_line_index = cur_line_index + 1
        if s.find('# un.onDeleteFiles NsisScriptGenerate.py') != -1:
            insert_uninstall_line_index = cur_line_index + 1
    f.close()

    # Insert install extract files
    if insert_install_line_index == -1:
        return

    for s in g_insert_nsis_script_install_list:
        all_nsis_script_lines.insert(insert_install_line_index, s)
        insert_install_line_index += 1
        insert_uninstall_line_index += 1

    # Insert uninstall delete files
    if insert_uninstall_line_index == -1:
        return

    for s in g_insert_nsis_script_uninstall_list:
        all_nsis_script_lines.insert(insert_uninstall_line_index, s)
        insert_uninstall_line_index += 1

    #Insert REQUIRED_SPACE Macro
    all_nsis_script_lines.insert(24, '!define REQUIRED_SPACE        {0}\n'.format(g_root_dir_size))
    print('required space: {0:.2f}'.format(g_root_dir_size / 1024 / 1024))
    nsis_script_path = nsis_script_template_path[:len(nsis_script_template_path) - len('_template.nsi')]
    nsis_script_path += '.nsi'
    f = open(nsis_script_path, "w")
    f.writelines(all_nsis_script_lines)
    f.close()

if __name__ == '__main__':
    nsis_script_template_path = sys.argv[1]
    nsis_script_template_path = os.path.abspath(nsis_script_template_path)
    print(nsis_script_template_path)

    g_root_dir = sys.argv[2]
    g_root_dir = os.path.abspath(g_root_dir)
    print(g_root_dir)
    
    do_main(nsis_script_template_path)