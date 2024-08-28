# This is user manual

For full documentation visit [mkdocs.org](https://www.mkdocs.org).

## 1

* `mkdocs new [dir-name]` - Create a new project.
* `mkdocs serve` - Start the live-reloading docs server.
* `mkdocs build` - Build the documentation site.
* `mkdocs -h` - Print help message and exit.

## Enclosure manufacturing

### FDM printing

Make sure that you flip the objects to have the falt surface on the bed, no support is required. 
Fits conformatbly on the 18x18cm bed. 
On my prusa Mini+, the top shell takes 2 hour to print on a 0.15mm layer height. 2:16 for the bottom

    mkdocs.yml    # The configuration file.
    docs/
        index.md  # The documentation homepage.
        ...       # Other markdown pages, images and other files.
