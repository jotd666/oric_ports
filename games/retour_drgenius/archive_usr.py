import os, make_archive_no_cc as make_archive,which



m = make_archive.MakeArchive()


m.add_file("retour_dr_genius.exe")
for dll in ["SDL.dll"]:
    src = which.which(dll)[0]
    m.add_file(src)

#m.add_directory("snd")
m.add_file("readme.txt")
m.add_file("solution.txt")
#m.add_buffer_as_file("strip21.bat",run,main_directory,text_mode=True)
m.create_zip(r"D:\jff\data\webpage\oric_conversions\RetourDrGenius.zip")
