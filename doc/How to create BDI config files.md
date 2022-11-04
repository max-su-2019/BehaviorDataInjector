# How To Create BDI Config File

First of all, create a json format config file with suffix named "_BDI.json"(for example, "MyTestMod_BDI.json") under the path "SKSE\Plugins\BehaviorDataInjector".  

Next, open the config file we created with a codes editor(I recommend [Visual Studio Code](https://code.visualstudio.com/)) that supports json format files, and write config data for it.  

Let's take "Test_BDI.json" in BDI default files as reference, to illustrate how to write vaild config codes for BDI:

```json
[   
    {
        "projectPath": "Actors",
        "type": "kInt",
        "name": "MaxsuTestInt",
        "value": 123
    },
    
    {
        "projectPath": "actors\\Character",
        "type": "kBool",
        "name": "MaxsuTestBool",
        "value": true
    },

    {
        "projectPath": "Actors\\Horse",
        "type": "kFloat",
        "name": "MaxsuTestFloat",
        "value": 123.12
    },

    {
        "projectPath": "Actors",
        "type": "kEvent",
        "name": "MaxsuTestEvent"     
    }
]
```

Config file uses standard [json syntax](https://www.w3schools.com/js/js_json_syntax.asp): The square bracket in the beginning and end is used to define a json array, inside the array stores the BDI data objects. Between each curly brackets an BDI object was stored, and each object have to be separated with a comma.  

Inside a BDI object, there are four keys we need to assign value for:
* **projectPath**: Specify the path that the behavior project files located in, all the behavior files under this path would be injected with this graph variable / animation event. The path is relative to "Data\Meshes\", therefore "Actros\Character" actually points to  "Data\Meshes\Actros\Character".  
  
* **type**: The data type of the BDI object, there are four types are available: kInt, KBool, kFloat, kEvent.

* **name**: The name of the grpah variable or animation event.

* **value**: The initial value of the BDI object, for animation event this key is not necessary  
<br/>

## Debuging  
You can set the option "EnableDebugLog" in "BehaviorDataInjector.ini" to dispaly debug message.  
There is also a console command to help you check if a graph variable is valid:
```
DGV [VariableName] -type
```

For instance, select an actor, then enter command: `DGV MaxsuTestInt i`, that would display the value of the graphVariableint if it's existed.
