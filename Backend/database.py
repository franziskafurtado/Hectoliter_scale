import peewee

db = peewee.SqliteDatabase('hectoliter.sql', pragmas={'foreign_keys':1})


class BaseModel(peewee.Model):
    class Meta:
        database = db

class Machine(BaseModel):
    runningProcess = peewee.IntegerField(null=True) #Não dá pra usar ForeignKey aqui pq a classe process ainda não foi declarada - Usamos gambiarra aqui
    uuid = peewee.CharField(max_length=32, unique=True)
    secret = peewee.CharField(max_length=256)
    status = peewee.IntegerField()
    lastUpdate = peewee.DateTimeField()
    newProcess = peewee.BooleanField()

class User(BaseModel):
    username = peewee.CharField(max_length=120, unique=True)
    password = peewee.CharField(max_length=72)
    name = peewee.CharField(max_length=255)

class Process(BaseModel):
    status = peewee.IntegerField()
    timestamp = peewee.DateTimeField(null=True)
    machine = peewee.ForeignKeyField(Machine)
    user = peewee.ForeignKeyField(User)
    weight = peewee.DecimalField(null=True)
    hectoliter = peewee.DecimalField(null=True)

class Error(BaseModel):
    id = peewee.IntegerField(primary_key=True)
    description = peewee.TextField()

class MachineError(BaseModel):
    machine = peewee.ForeignKeyField(Machine, backref='errors')
    error = peewee.ForeignKeyField(Error)

class ProcessError(BaseModel):
    process = peewee.ForeignKeyField(Process, backref='errors')
    error = peewee.ForeignKeyField(Error)

db.create_tables([Machine, Process, User, Error, MachineError, ProcessError])