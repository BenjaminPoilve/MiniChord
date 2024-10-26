# Documentation 

This folder contains all the necessary files to build the documentation 

## Installation of mkdocs 
To install using python3, follow those steps:

* `pip3 install mkdocs`
* `pip3 install mkdocs-terminal` 
* add `/Users/{user_name}/Library/Python/3.8/binv` to `/etc/paths`  (on OSX)

## Usage 

Modifications should be made in the [docs folder](https://github.com/BenjaminPoilve/MiniChord/tree/main/documentation/docs).

To be able to have a live view, use the `mkdocs serve` command and open the indicated URL in your browser. 

The home page itself is coded manually (the `index.html`file). 

Once changes are made use the `build_site.sh` script to build the full website (this scripts builds the mkdocs files then copies the additional ressources).