from datetime import datetime
from random import randint
from time import sleep
import json

template = {
    "time": "",
    "response": {
        "body": "what",
        "status": {
            "code": 400,
            "message": "Bad Request"
        }
    }
}

f = open("mylog", "a+")
statuses = [200,400,401,502]

for i in range(0, 1000000):

    template["time"] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    template["response"]["status"]["code"] = statuses[randint(0,3)]

    sleep(randint(0,2))
    print(i)

    f.writelines(json.dumps(template) + "\n")
    f.flush()

f.close()
