# Dnd2Install

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Shell Script](https://img.shields.io/badge/shell_script-%23121011.svg?style=for-the-badge&logo=gnu-bash&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

A drag and drop installer for Linux.

<p align="center">
  <img src="screenshot.png" alt="Screenshot" />
</p>

## Get started

### Install

- Download package from [Releases](https://github.com/eduhds/dnd2install/releases)

```sh
# Debian
wget https://github.com/eduhds/dnd2install/releases/download/v1.0.1/dnd2install-v1.0.1-x86_64.deb
sudo apt install -y ./dnd2install*.deb

# Fedora
wget https://github.com/eduhds/dnd2install/releases/download/v1.0.1/dnd2install-v1.0.1-x86_64.rpm
sudo dnf install -y ./dnd2install*.rpm
```

### Usage

1. Right click on tarball file
2. Click "Open with" > "Drag and drop to install"

## Development

```sh
# Building
bash scripts/build.sh -d # debug
bash scripts/build.sh -r # release

# Packaging
bash scripts/package.sh --deb
bash scripts/package.sh --rpm
```

## Credits

- [webview/webview](https://github.com/webview/webview)
- <a  href="https://icons8.com/icon/kTr9DhKidICy/packing">Packing</a>, <a  href="https://icons8.com/icon/gGQqaCyJ5Auh/empty-box">Empty Box</a>, <a  href="https://icons8.com/icon/SauwBNNErOqo/opened-folder">Opened Folder</a> icons by <a href="https://icons8.com">Icons8</a>
