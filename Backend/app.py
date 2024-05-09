import flask
import database
import re
import datetime
import hmac
import hashlib
import base64
import json
import bcrypt

app = flask.Flask(__name__)
secret_key = 'MIIBVgIBADANBgkqhkiG9w0BAQEFAASCAUAwggE8AgEAAkEAtMg2igXuDcgpOTkeRvEppjaNy72FzNNP8wV9dS/Pux3QqzdheqP+qSJCtzS2SVV9P2VdYGUuOAuaLj1vnGcifwIDAQABAkEAn0vcYvqerR421OlzuGdAn+lqQKEbiUSaKjYoOl6K2QENgY9t9C12CREC0Wp+ypZhJNp7DJdZUSJVJ+cd04GjYQIhANcniN621s8Qz7LjXiWHZ12kcyP2jCWYHnx+EyisNn/tAiEA1xowRv3FTZGoz9R9CNpgsUdgPEE2cERygVncemvEppsCIQCiMaaHprQm4xiAVdD6X6n3uOon2UvrZ1LMLMrKpZBsPQIgeIzTy6WDgxKEEl3a6TBCCnifBYXitY6WwcmT2AQ3xMsCIQC7BePcXT47u9fUG1Xa7MYYpKW1wY5iOQQ8FevqRNYNIA=='

@app.route('/machineSync', methods=['GET','POST'])
def machineSync():
    if flask.request.method == 'GET':
        return flask.jsonify({'error':'Machine sync must be done via POST'}), 400

    print(flask.request.json)
    if not 'uuid' in flask.request.json or flask.request.json['uuid'] == '' or not 'secret' in flask.request.json or flask.request.json['secret'] == '':
        return flask.jsonify({'error':'Authentication failed'}), 401

    try:
        machine = database.Machine.get(uuid=flask.request.json['uuid'])
    except database.peewee.DoesNotExist:
        return flask.jsonify({'error':'Authentication failed'}), 401

    if machine.secret != flask.request.json['secret']:
        return flask.jsonify({'error':'Authentication failed'}), 401

    if not 'status' in flask.request.json or not re.match("^(0|1|2|3)$",str(int(flask.request.json['status']))):
        return flask.jsonify({'error':'Invalid status'}), 400

    status = int(flask.request.json['status'])

    #Limpa erros
    for error in machine.errors.select():
        error.delete_instance()

    if status == 0:
        machine.status = 0
        machine.lastUpdate = datetime.datetime.now()
        machine.save()

    elif status == 1:
        if not machine.runningProcess:
            return flask.jsonify({'error':'There is no process set as running'}), 400
        machine.status = 1
        machine.lastUpdate = datetime.datetime.now()
        machine.newProcess = False
        machine.save()

    elif status == 2:
        if not 'errors' in flask.request.json or not isinstance(flask.request.json['errors'], list) or len(flask.request.json['errors']) == 0:
             return flask.jsonify({'error':'No errors codes were sent'}), 400
        
        for error in flask.request.json['errors']:
            try:
              error = int(error)
            except:
              return flask.jsonify({'error':'Error code '+ error+ ' invalid'}), 400

            try:
                err = database.Error.get(id=error)
            except database.peewee.DoesNotExist:
                return flask.jsonify({'error':'Error code '+str(error)+' invalid'}), 400

        for error in flask.request.json['errors']:
            machineerror = database.MachineError()
            machineerror.machine = machine
            machineerror.error = database.Error.get(id=int(error))
            machineerror.save()

        machine.status = 2
        machine.lastUpdate = datetime.datetime.now()
        machine.save()
        
    elif status == 3:
        if not machine.runningProcess:
           return flask.jsonify({'error':'There is no process set as running'}), 400

        try:
            process = database.Process.get(id=machine.runningProcess)
        except database.peewee.DoesNotExist:
            return flask.jsonify({'error':'There is no process set as running'}), 400

        if process.status != 0:
            return flask.jsonify({'error':'Process already finished'}), 400

        if not 'process' in flask.request.json or not isinstance(flask.request.json['process'], dict):
            return flask.jsonify({'error':'Process must be sent'}), 400

        processData = flask.request.json['process']

        try:
            processStatus = int(processData['status'])
        except:
            return flask.jsonify({'error': 'Invalid process status'}), 400

        if not re.match("^(1|2)$",str(processStatus)):
            return flask.jsonify({'error': 'Invalid process status'}), 400

        if (not 'weight' in processData or
           processData['weight'] == '' or
           not 'hectoliter' in processData or
           processData['hectoliter'] == ''):
            return flask.jsonify({'error':'Process parameters are missing'}), 400

        if int(processData['status'] == 2) and not 'errors' in processData or not isinstance(processData['errors'], list) or len(processData['errors']) == 0:
            return flask.jsonify({'error':'No errors codes were sent'}), 400

        for error in processData['errors']:
            try:
              error = int(error)
            except:
              return flask.jsonify({'error':'Error code '+ error+ ' invalid'}), 400

            try:
                err = database.Error.get(id=error)
            except database.peewee.DoesNotExist:
                return flask.jsonify({'error':'Error code '+str(error)+' invalid'}), 400

        try:
            weight = float(processData['weight'])
        except:
            return flask.jsonify({'error':'weight invalid'}), 400

        try:
            hectoliter = float(processData['hectoliter'])
        except:
            return flask.jsonify({'error':'hectoliter invalid'}), 400

        for error in processData['errors']:
            processerror = database.ProcessError()
            processerror.process = process
            processerror.error = database.Error.get(id=int(error))
            processerror.save()

        process.status = processStatus
        process.timestamp = datetime.datetime.now()
        process.weight = weight
        process.hectoliter = hectoliter
        machine.status = 3
        machine.lastUpdate = datetime.datetime.now()
        machine.runningProcess = None
        process.save()
        machine.save()

    ret = {
      "uuid": machine.uuid,
      "status": machine.status,
      "lastUpdate": machine.lastUpdate.isoformat()
    }

    retErr = []
    for error in machine.errors:
        retErr.append({
          "id": error.error.id,
          "description": error.error.description
        })

    if(len(retErr) > 0):
        ret['errors'] = retErr

    if(machine.runningProcess):
        ret['runningProcess'] = machine.runningProcess

    if(machine.status == 0 or machine.status == 3):
        ret['newProcess'] = machine.newProcess

    return flask.jsonify(ret)
        
def generateJWT(user):
    header = json.dumps({
        "typ": "JWT",
        "alg": "HS256"
    }).encode()

    expiration = datetime.datetime.now() + datetime.timedelta(days=1)
    payload = json.dumps({
        "username": user.username,
        "expiration": datetime.datetime.timestamp(expiration)
    }).encode()

    b64_header = base64.urlsafe_b64encode(header).decode()
    b64_payload = base64.urlsafe_b64encode(payload).decode()

    sig = hmac.new(
        key=secret_key.encode(),
        msg=f'{b64_header}.{b64_payload}'.encode(),
        digestmod=hashlib.sha256
    ).digest()

    token = f'{b64_header}.{b64_payload}.{base64.urlsafe_b64encode(sig).decode()}'

    return token

def checkJWT(token):
    parts = token.split('.')

    if len(parts) != 3:
        raise Exception('Invalid token')

    b64_header = parts[0]
    b64_payload = parts[1]
    sig = parts[2]

    if sig != base64.urlsafe_b64encode(hmac.new(
        key=secret_key.encode(),
        msg=f'{b64_header}.{b64_payload}'.encode(),
        digestmod=hashlib.sha256
        ).digest()).decode():
        raise Exception('Invalid token')

    payload = json.loads(base64.urlsafe_b64decode(b64_payload))

    if datetime.datetime.timestamp(datetime.datetime.now()) > payload['expiration']:
        raise Exception('Expired token')

    try:
        user = database.User.get(username=payload['username'])
    except database.peewee.DoesNotExist:
        raise Exception('User does not exist')

    return user

@app.route('/login', methods=['GET','POST'])
def login():
    if flask.request.method == 'GET':
       return flask.jsonify({'error':'Login must be done via POST'}), 400

    if not 'username' in flask.request.form or flask.request.form['username'] == '' or not 'password' in flask.request.form or flask.request.form['password'] == '':
         return flask.jsonify({'error':'Username and password must be sent'}), 400

    try:
        user = database.User.get(username=flask.request.form['username'])
    except database.peewee.DoesNotExist:
        return flask.jsonify({'error':'Username or password inválid'}), 401

    passwd = user.password.encode('utf-8')

    if not bcrypt.hashpw(flask.request.form['password'].encode('utf-8'),passwd) == passwd:
        return flask.jsonify({'error':'Username or password inválid'}), 401

    return flask.jsonify({
        "token": generateJWT(user)
    })

@app.route('/machines')
def getMachines():
    if not 'Token' in flask.request.headers:
        return flask.jsonify({'error':'User not authenticated'}), 401

    try:
        checkJWT(flask.request.headers['Token'])
    except Exception as e:
        return flask.jsonify({'error': str(e)}), 401

    ret = []
    machines = database.Machine.select()

    for machine in machines:
        ret.append({
            "id": machine.id,
            "status": machine.status,
            "lastUpdate": machine.lastUpdate.isoformat()
        })

    return flask.jsonify(ret)

@app.route('/machines/<int:id>')
def getMachine(id):
    if not 'Token' in flask.request.headers:
        return flask.jsonify({'error':'User not authenticated'}), 401

    try:
        checkJWT(flask.request.headers['Token'])
    except Exception as e:
        return flask.jsonify({'error': str(e)}), 401

    try:
        machine = database.Machine.get(id=id)
    except database.peewee.DoesNotExist:
        return flask.jsonify({'error':'Machine not found'}), 404

    ret = {
        "id": machine.id,
        "status": machine.status,
        "lastUpdate": machine.lastUpdate.isoformat()
    }

    if machine.status == 2:
        retErr = []
        for error in machine.errors:
            retErr.append({
                "id": error.error.id,
                "description": error.error.description,
            })

        ret['errors'] = retErr
    if machine.runningProcess:
        ret['runningProcess'] = machine.runningProcess
    return flask.jsonify(ret)

@app.route('/processes')
def getProcesses():
    if not 'Token' in flask.request.headers:
        return flask.jsonify({'error':'User not authenticated'}), 401

    try:
        checkJWT(flask.request.headers['Token'])
    except Exception as e:
        return flask.jsonify({'error': str(e)}), 401

    processes = database.Process.select().where(database.Process.status!=0)

    if 'start' in flask.request.args and flask.request.args['start'] != '':
        try:
            startDate = datetime.datetime.strptime(flask.request.args['start'], '%Y-%m-%d')
        except:
            return flask.jsonify({'error':'Invalid start date'}), 401

        processes = processes.where(database.Process.timestamp>=startDate)

    if 'end' in flask.request.args and flask.request.args['end'] != '':
        try:
            endDate = datetime.datetime.strptime(flask.request.args['end'], '%Y-%m-%d') + datetime.timedelta(days=1) #Coloca o dia seguinte as 0:00 para pegar tudo que for antes disso
        except:
            return flask.jsonify({'error':'Invalid end date'}), 401

        processes = processes.where(database.Process.timestamp<endDate)

    ret = []

    for process in processes:
        ret.append({
            "id": process.id,
            "status": process.status,
            "timestamp": process.timestamp.isoformat(),
            "user": process.user.name
        })

    return flask.jsonify(ret)

@app.route('/processes/<int:id>')
def getProcess(id):
    if not 'Token' in flask.request.headers:
        return flask.jsonify({'error':'User not authenticated'}), 401

    try:
        checkJWT(flask.request.headers['Token'])
    except Exception as e:
        return flask.jsonify({'error': str(e)}), 401

    try:
        process = database.Process.get(id=id)
    except database.peewee.DoesNotExist:
        return flask.jsonify({'error':'Process not found'}), 404

    ret = {
        "id": process.id,
        "status": process.status,
        "timestamp": process.timestamp.isoformat(),
        "user": process.user.name
    }

    errList = []
    if process.status == 2:
        for error in process.errors:
            errList.append({
                "id": error.error.id,
                "description": error.error.description,
            })
        ret['error'] = errList

    return flask.jsonify(ret)

@app.route('/machines/<int:id>/start', methods=['GET', 'POST'])
def startProcess(id):
    if flask.request.method == 'GET':
       return flask.jsonify({'error':'Process start must be done via POST'}), 400

    if not 'Token' in flask.request.headers:
        return flask.jsonify({'error':'User not authenticated'}), 401

    try:
        user = checkJWT(flask.request.headers['Token'])
    except Exception as e:
        return flask.jsonify({'error': str(e)}), 401

    try:
        machine = database.Machine.get(id=id)
    except database.peewee.DoesNotExist:
        return flask.jsonify({'error':'Machine not found'}), 404

    timeout = datetime.datetime.now() - machine.lastUpdate

    if timeout.total_seconds() > 600:
        return flask.jsonify({'error':'Machine is offline'}), 409

    if not (machine.status == 0 or machine.status == 3):
        return flask.jsonify({'error':'Machine is not available to start a new process'})

    #Create new process
    process = database.Process()
    process.status = 0
    process.machine = machine
    process.user = user
    process.save()

    machine.runningProcess = process.id
    machine.newProcess = True 
    machine.save()

    return flask.jsonify({"message":"ok"})