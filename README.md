# Fabamaq

To compile this code run cmake. The is a script that tries to install all potentially required dependencies, this script uses the apt package manager, so if you use another package manager you will need to edit it. Some dependencies might be missing depending on your particular hardware, due to the hardware that I have being limited in variety only a few configurations were tested, so there might be dependencies that are required and are not in that script. For optimal chances of compiling with no problem I recommend using Ubuntu.

For windows there should be no problems as all required dependencies should be in the ThirdParty folder. Do keep in mind that if you use cmake in a separate folder not all files of the ThirdParty folder are copied and you might be missing some lib's particularly for glfw, in this case I recommend just copying over the glfw folder to the build folder/ThirdParty. If you use cmake on the root you should have no issues compiling.

The resources must be present wherever you run the executable from, as it contains texture and shaders required.