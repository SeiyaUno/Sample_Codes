Search Engine Instructions (engine.py)
To save time, the "doc-ids.p" file, "index-offsets.p" file and "index" folder (built off the DEV domain) have already been included
If new files have been built using index.py make sure they are in the same directory as engine.py and replace the included files
Start the search engine by running "engine.py" 
To search, input queries in the console and press enter (displays top 20 results and search time)
Enter "quitprog" into the console to exit the search engine

Index Instruction (index.py)
Set corpus path at line 298 to your domain folder path.
Make sure the domain folder is in same directory as index.py and engine.py
Run index.py to construct the index.
A "doc-ids.p" file, "index-offsets.p" file and "index" folder will be created in the directory.

Web GUI Instructions (api.py, window.html)
Install Flask
Run api.py
Then open window.html in broswer to use the web GUI.
Displays top 100 results and search time.
Enter "CTRL+C" command in api.py console to exit.
