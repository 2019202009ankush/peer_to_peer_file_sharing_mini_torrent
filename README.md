# Mini Torrent ( P2P file sharing )
# Architechture
[![Ankush1-Page-3.png](https://i.postimg.cc/3JqKxkVY/Ankush1-Page-3.png)](https://postimg.cc/MvykspzF)
# Important command
## Run Tracker:
./tracker tracker_info.txt
   tracker_info.txt - Contains ip, port details of all the trackers
## Run Client:
./client <IP>:<PORT> tracker_info.txt
  tracker_info.txt - Contains ip, port details of all the trackers
## Create User Account:
create_user <user_id> <passwd>
## Login:
login <user_id> <passwd>
## Create Group:
create_group <group_id>
## Join Group:
join_group <group_id>
## Leave Group:
leave_group <group_id>
## List All Group In Network:
list_groups
## List All sharable Files In Group:
list_files <group_id>
## Upload File:
upload_file <file_path> <group_id>
## Download File:
download_file <group_id> <file_name> <destination_path>
## Logout:
logout
