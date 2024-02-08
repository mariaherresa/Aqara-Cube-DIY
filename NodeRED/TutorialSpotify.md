# Cómo configurar el nodo de Spotify

## Crear cuenta de Autor
- Entramos en Spotify for Developers e iniciamos sesión con nuestra cuenta premium
- Seleccionamos en nuestro perfil la opción Dashboard y creamos una app. En la opcción de qué clase de API planeas usar seleccionamos WebAPI
![image](https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/4edfdc62-a72d-45ab-93b9-ad36f09cd10c)

- Una vez creada, seleccionamos Dashboard -> Nuestra app -> Settings, y copiamos el client_ID y el secret code que nos proporciona
- Client_ID: 48a4613cfba545758b6af8b453cb8690
- Client secret: 0e2977d09b8643ac90f61da8637c9d10
- En cuanto al URI que nos piden, como estamos corriendo Node-RED en el localhost, este debería de ser: http://localhost:1880/spotify-credentials/auth/callback

## Configuración de cuenta en Node-RED
Ahora que hemos creado nuestra app, añadimos nuestra cuenta de autor. 

Para ello añadimos un nodo Spotify y en credenciales seleccionamos Add new spotify auth..., en este nodo solo se debe añadir el Client ID y el client secret mencionados anteriormente junto a los Scopes necesarios, que en nuestro caso los siguientes: 
ugc-image-upload
user-read-playback-state
user-modify-playback-state
user-read-currently-playing
app-remote-control
streaming
playlist-read-collaborative
playlist-modify-private
playlist-modify-public

Una vez añadido todo esto se presiona Authentification y nos redirige a la URI de la app que acabamos de crear para aceptar que nuestra cuenta de spotify se conecte con ella:
![image](https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/bb6ad652-0172-4208-86ed-d7c3f8ce2acf)

Al aceptar, spotify nos ha autorizado y nos debe de aparecer un mensaje como el siguiente y habríamos termiando de configurar el nodo (dependiendo del API que seleccionemos alomejor nos pide más información, pero en los APIs de este proyecto no es necesario)
![image](https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/2221cea5-756b-4c94-939f-40c9e8a0a6ab)

## Bibliografía
[Repositorio GitHub creador del nodo Spotify](https://github.com/pckhib/node-red-contrib-spotify)
[Documentación Spotify Web API](https://developer.spotify.com/documentation/web-api/tutorials/getting-started)
