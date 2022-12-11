# CreateRedirectors

![Icon128](https://user-images.githubusercontent.com/16429096/206930403-6ee52e5b-d29e-40af-b2f9-534b3fd5772d.png)

Create Redirectors is a plugin for the very specific use of manually creating asset redirectors in Unreal Engine.

This can be especially useful when you change the location of an asset that a lot of other assets where pointing to. By creating a manual redirector, you remove the necessity to checkout the assets referencing the asset you moved, reducing the amount of conflicts in you source control system.


## Install notes

* Clone plugin in your project's Plugins/ folder. 
* Regenerate visual studio project.
* Build your project using Visual Studio or Rider.
* Enable the plugin in the Plugin tab 
![image](https://user-images.githubusercontent.com/16429096/206930050-e978b454-b64d-4e69-ac2b-0cafb9e0ee35.png)
* Restart Unreal

## How to

### Create a redirector

* In the folder where you want to create a redirector, right click and choose "Create Redirector Here"

![image](https://user-images.githubusercontent.com/16429096/206929932-c226210a-0861-4bfd-b496-ab4194e82eda.png)

* A window will appear

![image](https://user-images.githubusercontent.com/16429096/206930292-1f2b9a9c-8942-427d-8d8e-3167f6f05e3b.png)

* Enter the asset you want a reference to

![image](https://user-images.githubusercontent.com/16429096/206930303-1ab3ef13-da3d-42a3-baae-2578f1ffdcb2.png)

* After creating the redirector, it should appear in your folder

![image](https://user-images.githubusercontent.com/16429096/206930330-436494f4-cc26-4168-bef1-c29a22a4b05f.png)

* If not, make sure you have the "Show Refirectors" filter on

![image](https://user-images.githubusercontent.com/16429096/206930346-54af5be5-e2e1-4d52-bbe4-e731f0f92727.png)


### Use this when moving an asset

There may be several reasons for you to create a redirector. 
One of them is to uncheckout a tons of assets that where referencing the asset you just moved.

* Move your asset from FolderA to FolderB
* Unreal will create a redirector in most cases, but if it doesnt you can create one yours
* Go to FolderA
* Create a redirector referencing your asset that is now in FolderB
* From your source control system, discard changes of the files that were auto changes when moving your asset, and make sure to stage the newly created redirector

# Credits 

This plugin is heavily relying on this code snippet by brifsttar
https://dev.epicgames.com/community/snippets/JW2/unreal-engine-create-redirector
