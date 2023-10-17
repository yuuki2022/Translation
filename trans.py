from transformers import MarianMTModel, MarianTokenizer
import sys
import os

def translate(text,model):
    model_name = model
    model = MarianMTModel.from_pretrained(model_name)
    tokenizer = MarianTokenizer.from_pretrained(model_name)

    # Tokenize the text
    tokenized_text = tokenizer.encode(text, return_tensors="pt")

    # Translate the tokenized text
    translated_tokens = model.generate(tokenized_text)

    # Decode the translated tokens to a string
    translated_text = tokenizer.decode(translated_tokens[0], skip_special_tokens=True)
    f = open("translated.log","w")
    f.write(translated_text)
    f.close()
    print(translated_text)
    return translated_text


def main():
        # Check if there are at least two arguments (script name and at least one additional argument)
    if len(sys.argv) < 3:
        print("Usage: python3 test.py --args <additional arguments>")
        return
    additional_arguments = sys.argv[2]
    
    if (sys.argv[1] == "en"):
    # en to zh
        return(str(translate(str(additional_arguments),str('Helsinki-NLP/opus-mt-en-zh'))))
    elif (sys.argv[1] == "zh"):
        return(str(translate(str(additional_arguments),'Helsinki-NLP/opus-mt-zh-en')))
    else:
        return ""
if __name__ == "__main__":
    	main()
