import os
import sys

dependent_files_debug = ('Qt5Cored.dll', 'Qt5Guid.dll', 'Qt5Widgetsd.dll', 
                         'qgifd.dll', 'qicnsd.dll', 'qicod.dll', 'qjpegd.dll',
                         'qtgad.dll', 'qtiffd.dll', 'qwbmpd.dll', 'qwebpd.dll',
                         'qwindowsd.dll',
                         'qwindowsvistastyled.dll')

dependent_files_release = ('Qt5Core.dll', 'Qt5Gui.dll', 'Qt5Widgets.dll', 
                           'qgif.dll', 'qicns.dll', 'qico.dll', 'qjpeg.dll',
                           'qtga.dll', 'qtiff.dll', 'qwbmp.dll', 'qwebp.dll',
                           'qwindows.dll',
                           'qwindowsvistastyle.dll')





def DeleteFiles(path,file_list) :
    # delete files
    for root, dirs, files in os.walk(path) :
        for f in files:
            full_path = os.path.join(root, f)
            if (f not in file_list) :
                try:
                    os.remove(full_path)
                    print('delete files: ',full_path)
                except:
                    print('delete files: ',full_path, ' failed.')

    # remove empty directory
    for root, dirs, files in os.walk(path) :
        for d in dirs:
            full_path = os.path.join(root, d)
            if not os.listdir(full_path) :
                try :
                    os.rmdir(full_path)
                    print('remove directory: ', full_path)
                except :
                    print('remove directory: ', full_path,' failed.')



if __name__ == '__main__':

    file_path = sys.argv[1]
    print(file_path)

    type = sys.argv[2]
    print(type)

    if(type == 'Debug') :
        DeleteFiles(file_path,dependent_files_debug)
    else :
        DeleteFiles(file_path,dependent_files_release)
