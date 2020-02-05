# XamppRoot
XamppRoot is a command-line tool built to easily modify Xampp's localhost root folder.

[![BuyMeACoffee](https://img.shields.io/badge/coffee-donate-yellow.svg?logo=buy-me-a-coffee)](https://buymeacoff.ee/alecarabelli)
[![Chocolatey](https://img.shields.io/chocolatey/dt/xampproot?color=brightgreen&label=Chocolatey%20downloads)](https://chocolatey.org/packages/xampproot)

## Requirements
Only working for Windows systems.

For a correct functioning, xampp needs to be installed on the PC.

The default Xampp installing folder is assumed to be C:\\Xampp

## Flags

| Flag   | Argument required | Functionality                           |
|--------|-------------------|-----------------------------------------|
| -l     | No                | Set root folder to the executing folder |
| -p     | Yes               | Path to the folder (can be an absolute or relative path) |
| -x     | Yes               | Modifies Xampp root folder              |

## Usage

### Chocolatey package
Installation and update with [Chocolatey package manager](https://chocolatey.org/packages/xampproot/)
- Install package (`choco install xampproot`)
- Close and re-open terminal
- Type `xampproot` followed by the needed flags
- Enjoy 😁

### Local installation
- Download and compile the project
- Move the .exe in a suitable folder
- Open terminal (administrative permits aren't required)
- Type `xampproot` followed by the needed flags
- Enjoy 😁

## Examples
Xampp in default folder, wanted localhost root in executing folder
```
xampproot -l
```


Xampp in default folder, wanted localhost root in specified path
```
xampproot -p Desktop\htmlproject

xampproot -p ..\parentfolder\myfolder

xampproot -p D:\projects\mysite
```

Xampp _NOT_ in default folder
```
xampproot -l -x D:\mypath\Xampp

xampproot -p path\to\folder -x path\to\xampp
```