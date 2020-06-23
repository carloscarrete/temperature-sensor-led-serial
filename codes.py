import serial
import threading
import time
import re
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

arduino = serial.Serial('/dev/ttyS1',baudrate=9600,timeout=1.0)
envio = True
def waitingForWrite(caracter):
    while True:
        valorDeEntrada = input()
        print(valorDeEntrada)
        writeX = arduino.write(valorDeEntrada.encode())
        time.sleep(1)
def mandarCorreo(temperatura):
    receptores = ['17040434@itdurango.edu.mx','carretecarlosg@gmail.com']
    email = 'cuenta.prueba.raspberry@gmail.com'
    password = 'tecno12345'
    subject = '¡Aviso!. La temperatura actual del invernadero se encuentra en un estado crítico, por favor toma las medidas pertinentes. La temperatura actual es de: ' +temperatura
    send_to_email = ', '.join(receptores)
    message = 'De nuevo molestando con esta pequeña pruebita'

    msg = MIMEMultipart()
    msg['From'] = email
    msg['To'] = send_to_email
    msg['Subject'] = subject


    msg.attach(MIMEText(message, 'plain'))

    server = smtplib.SMTP('smtp.gmail.com', 587)
    server.starttls()
    server.login(email, password)
    text = msg.as_string() 
    server.sendmail(email, receptores, text)
    server.quit()
    

hilo = threading.Thread(target=waitingForWrite,args=("Waiting",))
hilo.start()
while True:
    line = arduino.readline()
    lineString = str(line)
    expresionRegular = "\d+\.\d+"
    temperatura = expresionRegular
    founded = re.search(expresionRegular,lineString)
    if founded is not None:
            foundedConverterArray = founded[0]
    if "FAL" in lineString:
        if(envio):
            mandarCorreo(foundedConverterArray)
            envio = False
    if(line.decode()!=''):
        if "FAL" in line.decode():
            print(foundedConverterArray," C")
        else:
            print(line.decode('utf-8'))

    
    


    
