---
title: Отправка данных в Thingsboard
updated: 2021-11-10 15:21:11Z
created: 2021-11-10 14:47:12Z
latitude: 48.71680000
longitude: 44.50350000
altitude: 0.0000
---

**ЧТО НЕОБХОДИМО ДЛЯ ОТПРАВКИ ДАННЫХ НА IoT-ПЛАТФОРМУ THINGSBOARD**

<ins>**Адрес платформы (брокера в терминологии MQTT):**</ins> 89.208.220.227 <ins>**Порт:**</ins> 1883<br>
<ins>**Токен выданный платформой устройству:**</ins> TOKEN BYtlzAIaJfGP5CAsBkI3<br>
<ins>**Точка входа API (topic в терминологии MQTT):**</ins> v1/devices/me/telemetry<br>
<ins>**Инструмент для отправки данных по протоколу MQTT**</ins>:<br>
  mosquitto_pub -d -q 1 -h "89.208.220.227" -t "v1/devices/me/telemetry" -u "BYtlzAIaJfGP5CAsBkI3" -f "payload.json"

<ins>**Сами данные:**</ins>
  {
  "temperature":22,
  "humidity":45
  }

<ins>**Документация по этой теме:**</ins>
https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api

<ins>**Документация в целом по платформе:**</ins>
https://thingsboard.io/docs/
