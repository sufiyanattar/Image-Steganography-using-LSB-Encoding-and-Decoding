#Introduction

Steganography is the art of hiding the fact that communication is taking place, by hiding information in other information. Many carrier file formats can be used, but digital images are the most popular because of their frequency on the internet. For hiding secret information in images, there exists a large variety of steganography techniques some are more complex than others and all of them have respective strong and weak points. Different applications may require absolute invisibility of the secret information, while others require a large secret message to be hidden.

 Steganography is the practice of hiding private or sensitive information within something that appears to be nothing out of the usual. Steganography is often confused with cryptology because the two are similar in the way that they both are used to protect important information. The difference between two is that steganography involves hiding information so it appears that no information is hidden at all. If a person or persons views the object that the information is hidden inside he or she will have no idea that there is any hidden information, therefore the person will not attempt to decrypt the information.

What steganography essentially does is exploit human perception, human senses are not trained to look for files that have information inside them, although this software is available that can do what is called Steganography. The most common use of steganography is to hide a file inside another file.

#Requirement Details

The application accepts an image file say .bmp along with a text file that contains the message to be steged

Analyze the size of the message file to check whether the message could fit in the provided .bmp image

Provide an option to steg a magic string which could be useful to identify whether the image is steged or not

The application should provide an option to decrypt the file

This has to be a command-line application and all the options have to be passed as a command-line argument

#Sample Output

Fig1: Command Line usage with arguments

![ls_ss1](https://github.com/sufiyanattar/Image-Steganography-using-LSB-Encoding-and-Decoding/assets/109298044/3beff9e8-fcfe-4a41-bcd3-af836ee009e1)

Fig2: Encoding usage via command line

![ls_ss2](https://github.com/sufiyanattar/Image-Steganography-using-LSB-Encoding-and-Decoding/assets/109298044/935da7c2-80a4-4787-9fe7-d404de24eb1c)

Fig3: Encoding steps and output at different stages

![ls_ss3](https://github.com/sufiyanattar/Image-Steganography-using-LSB-Encoding-and-Decoding/assets/109298044/fa391c77-5b72-4757-9530-191b2fe5d0dd)

Fig4: Decoding steps and output

![ls_ss5](https://github.com/sufiyanattar/Image-Steganography-using-LSB-Encoding-and-Decoding/assets/109298044/4cd71012-9fdc-438b-b155-5f670897c6ed)
