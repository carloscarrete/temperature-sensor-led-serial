import serial
import threading
import time
import re
from pymongo import MongoClient 
from datetime import datetime
from twilio.rest import Client
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

arduino = serial.Serial('/dev/ttyS1',baudrate=9600,timeout=1.0)

account_sid = 'ACdad7e74d2bb428de0dabc9a2388e9634'
auth_token = 'c53bef9c3023a30afafa810fedf14eee'
client = Client(account_sid, auth_token)

temperatura = 0
contadorTemPromedio1=0
foundedConverterArray=0
sumaContadorPromedio=0
promediodelContador=0
contadorMandarAlertas=0
ct = False
flag = False

def get_db():
    try:
        client = MongoClient("mongodb+srv://raspberry:tecno123@raspberry-ikf72.mongodb.net/<dbname>?retryWrites=true&w=majority")
        db = client.Temperatura
    except ConnectionFailure:
        print("Error de conexión")
    return db

def insertar(temperatura, fecha):
    db = get_db()
    resultado = db.TemperaturasDEMO.insert_one({"Temperatura":temperatura,"Fecha":fecha})
    return resultado.inserted_id


def waitingForWrite(caracter):
    try:
        valorDeEntrada = input()
        print(valorDeEntrada)
        writeX = arduino.write(valorDeEntrada.encode())
        time.sleep(5)
    except ValueError:
        print("FAIL")

def mensajeWhats(temp):
    message = client.messages.create(
                              body='¡Alerta!. Se ha detectado estado de fallo. Por favor, tome las medidas pertinentes. La temperatura es de: '+str(temp),
                              from_='whatsapp:+14155238886',
                              to='whatsapp:+5216181572410'
                          )
    
def mensajeSMS(temp):
    message = client.messages \
    .create(
         body='¡Alerta!. Se ha detectado estado de fallo. Por favor, tome las medidas pertinentes. La temperatura es: '+str(temp),
         from_='+12052930401',
         to='+526181572410'
     )

    
def mensajeMail(temp):
    email = 'cuenta.prueba.raspberry@gmail.com'
    password = 'tecno12345'
    send_to_email = 'cuenta.prueba.raspberry@gmail.com'
    subject = 'Estado de fallo' 
    message = '¡Alerta!. Se ha detectado estado de fallo. Por favor, tome las medidas pertinentes. La temperatura es: ' +str(temp)

    msg = MIMEMultipart()
    msg['From'] = email
    msg['To'] = send_to_email
    msg['Subject'] = subject

    msg.attach(MIMEText(message, 'plain'))

    server = smtplib.SMTP('smtp.gmail.com', 587)
    server.starttls()
    server.login(email, password)
    text = msg.as_string() 
    server.sendmail(email, send_to_email, text)
    server.quit()

get_db()
    
while True:
     if(contadorTemPromedio1<=10):
        line = arduino.readline()
        lineString = str(line)
        expresionRegular = "\d+\.\d+"
        temperatura = expresionRegular
        founded = re.search(expresionRegular,lineString)
        #print("Antes del fail", founded, temperatura, lineString)
        if(line.decode()!=''):
            print(line.decode('utf-8'))
        if founded is not None:
            foundedConverterArray = founded[0]
            print("PRUEBA ADV",foundedConverterArray)
            sumaContadorPromedio = sumaContadorPromedio+float(foundedConverterArray)
        else:
            flag = True
        if "FAL" in lineString:
            if contadorMandarAlertas == 15:
                mensajeWhats(float(foundedConverterArray))
                mensajeSMS(float(foundedConverterArray))
                mensajeMail(float(foundedConverterArray))
                flag = False
                contadorMandarAlertas=0
            else:
                contadorMandarAlertas = contadorMandarAlertas+1
        contadorTemPromedio1=contadorTemPromedio1+1
        print(contadorTemPromedio1)
        hilo = threading.Thread(target=waitingForWrite,args=("Waiting",))
        hilo.start()
     else:
        insertar(sumaContadorPromedio/10, datetime.now())
        print("El promedio de esta madre es de:",sumaContadorPromedio/10)
        print("La suma es:",sumaContadorPromedio)
            #print("Suma: ",sumaContadorPromedio)
        contadorTemPromedio1=0
        sumaContadorPromedio=0
    

