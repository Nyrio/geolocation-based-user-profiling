# Application module
Upper layer containing UI, including : 
- API for other module (GUI module)
- Terminal

## CLI Terminal 
When compiling in cli mode, the executable should be opened in terminal mode.
It accepts a possible parameter "-wp config/file.txt", check the README in ../data for more about it.

Other available commands are : 
- ```load *id*``` load and prepare data for the user *id*
- ```house``` display the 2 most-probable location of the house of the user lately loaded. 
- ```workplace``` display the most-probable location of the workplace
- ```show-clusters``` display all the cluster found
- ```frequent-places``` associate tags to the places visited by this user
- ```*exit*```  leave the console