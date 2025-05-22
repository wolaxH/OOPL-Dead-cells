import os
from colorama import Fore, Style
from natsort import natsorted

#Plaese check that your res are sorted, this Tool has no sort function
"""
    you have to install natsort. 
    if you dont have, type "pip install natsort" to install
"""

Source_Path = os.getcwd().replace("\\", "/") + "/source"   #The folder path that you will replac
Target_Path = os.getcwd().replace("\\", "/") + "/target"   #The folder path that you will save


prefix = "Atked_"    #The new name prefix eg: runB_{index}.png
errorItem = []
temp = input("Please input the prefix or press enter to use source files prefix:")
if (temp != ""):
    prefix = temp.replace("\n", "")+"_"

for i, old_name in enumerate(natsorted(os.listdir(Source_Path)), start = 0):
    old_path = os.path.join(Source_Path, old_name)
    new_name = f"{prefix}{i}{os.path.splitext(old_name)[1]}"
    new_path = os.path.join(Target_Path, new_name)

    try:
        os.rename(old_path, new_path)
        print(f"Rename {Fore.YELLOW + old_name + Style.RESET_ALL} to {Fore.YELLOW + new_name + Style.RESET_ALL} successes!")
    except:
        errorItem.append(old_name)

if (len(errorItem)):
    print(f"\n{len(errorItem)} occur")
    print("\n".join(item for item in errorItem))
else:
    print("complete with no error")
