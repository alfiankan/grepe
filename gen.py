from datetime import datetime
from random import uniform, randint
from time import sleep
import json

# template = {
#     "time": "",
#     "response": {
#         "body": "what",
#         "status": {
#             "code": 400,
#             "message": "Bad Request"
#         }
#     }
# }

f = open("mylog", "a+")
statuses = [200,400,401,502]

for x in range(0,10):

    for i in range(0, 1000):

        template = "203.0.113.10 - - [$log_time +0000] \"GET /index.html HTTP/1.1\" $status_code 1024 \"-\" \"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.190 Safari/537.36\""
        template = template.replace("$log_time", datetime.now().strftime("%d/%b/%Y:%H:%M:%S"))    
        template = template.replace("$status_code", str(statuses[randint(0,3)]))

    
        sleep(uniform(0.1, 0.2))
    
        f.writelines(template + "\n")

    f.flush()
    print("Iteration", x)

f.close()
