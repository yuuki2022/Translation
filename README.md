# Online Translation Platform
[中文](README-ZH.md)

**DISCLAIMER: This project is only tested and implemented on Linux, with no guarantee for other operating systems.**
# Introduction 
This is a translation platform where you can look up English words and translate English and Chinese bidirectionally.
![Look up a word](assets/look_up_word.png)
![English to Chinese, Credits: The Project Gutenberg eBook of Pride and Prejudice](assets/en-zh.png)
![Chinese to English, Credits: 再别康桥](assets/zh-en.png)

# Credits for Powered Features
Dictionary feature is powered by [ECDICT](https://github.com/skywind3000/ECDICT) by [Linwei](https://github.com/skywind3000).

Translation feature is powered by [opus-mt-en-zh](https://huggingface.co/Helsinki-NLP/opus-mt-en-zh) and [opus-mt-zh-en](https://huggingface.co/Helsinki-NLP/opus-mt-zh-en) by [Language Technology Research Group at the University of Helsinki](https://huggingface.co/Helsinki-NLP)

# Setup
1. Install `Qt Creator` (Qt Creator 6.0.2 is recommended).
2. Clone this repository.

## Server configuration

1. [Download](https://github.com/skywind3000/ECDICT/releases/tag/1.0.28) required database `ecdict-sqlite-28.zip` and extract `stardict.db` into `ServerTranslation` folder.

2. Open `ServerTranslation/ServerTranslation.pro` in `Qt Creator`, if everything goes well, you can build and run this project. 

**Please be aware that the server listens on port `8082`. Therefore, this port should not be occupied.**

3. (**For Translation Usage**) Install `Pytorch` from the following link:
https://pytorch.org/get-started/locally/

4. (**For Translation Usage**) Install Python library `transformers` and `sentencepiece`
```shell
pip install transformers sentencepiece
```

5. (Optional) Before running this project, running following python code
```shell
python3 trans.py en "Hello, World!" # Output should be "你好,世界!"
```
and
```shell
python3 trans.py zh "你好，世界！" # Output should be "Hello, world!"
```
to download and setup translation model.

## Client configuration
1. Open `OnlineTranslation/OnlineTranslation.pro` in `Qt Creator`.
2. Open `OnlineTranslation/mainwindow.cpp` and change the variable `const QString ip` to server's IP address.
3. If everything goes well, you can build and run this project.

# Usage

The user interface is simple and intuitive.
