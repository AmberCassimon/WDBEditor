# WDBEditor

This repository contains a very primitive application for interacting with World Database Files (Otherwise known as WDB files) used in the original Lego Island game.

<img src="res/img/Screenshot.png" style="max-width: 768px;" alt="A screenshot of the application."/>

## Status

Currently, despite its name, the files are currently read-only.
The application also only reads the first section of the file (Groups, Subgroups and Subitems), while ignoring most of the actual content.

## TODO
1. Add Serialization logic so files can be saved again after editing
2. Add UI support for more missing fields (Unknown 37 bytes in presenter info, for instance)
3. Add support for actual data chunks\
