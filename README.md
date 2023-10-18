# Online Translation Platform
[中文](README-ZH.md)

**DISCLAIMER: This project is only tested and implemented on Linux, with no guarantee for other operating systems.**
## Setup
1. Install `Qt Creator` (Qt Creator 6.0.2 is recommended)
2. Clone this repository

## Server configuration
1. Install `transformers` Python library 
```shell
pip install transformers
```
2. [Download](https://github.com/skywind3000/ECDICT/releases/tag/1.0.28) required database `ecdict-sqlite-28.zip` and extract `stardict.db` into `ServerTranslation` folder

3. Open `ServerTranslation/ServerTranslation.pro` in `Qt Creator`, if everything goes well, you can build and run this project. 

**Please be aware that the server listens on port `8082`. Therefore, this port should not be occupied.**

4. (Optional) Before running this project, running following python code
```shell
python3 trans.py en "Hello, World!" # Output should be "你好,世界!"
```
and
```shell
python3 trans.py zh "你好，世界！" # Output should be "Hello, world!"
```
to download and setup translation model

## Client configuation
1. Open `OnlineTranslation/OnlineTranslation.pro` in `Qt Creator`
2. Open `OnlineTranslation/mainwindow.cpp` and change the variable `const QString ip` to server's IP address
3. If everything goes well, you can build and run this project.

## Usage

The user interface is simple and intuitive.