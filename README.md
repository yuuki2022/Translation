# Online Translation Platform
[中文]()
**DISCLAIMER: This project is only implemented and tested on linux, no guarantee for other opreation systems.**
## Setup
1. Install `Qt Creator`
2. Clone this repository

## Server configuration
1. Install `transformers` Python library 
```
pip install transformers
```
2. [Download](https://github.com/skywind3000/ECDICT/releases/tag/1.0.28) required database `ecdict-sqlite-28.zip` and extract `stardict.db` into `ServerTranslation` folder

3. Open `ServerTranslation/ServerTranslation.pro` in `Qt Creator`, if everything goes well, you can build and run this project. **Beaware of that the program listens port `8082`, so this port shall not be occupied**

## Client configuation
1. Open `OnlineTranslation/OnlineTranslation.pro`, if everything goes well, you can build and run this project.

## Usage

The user interface is simple and intuitive.