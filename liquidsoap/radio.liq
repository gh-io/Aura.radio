# Live DJ stream
live = input.http("http://icecast:8000/live") 

# Playlist automation
playlist = playlist("/playlist.m3u")

# Mix live + automated fallback
radio = fallback(track_sensitive=true,[live, playlist])

# Send output to Icecast
output.icecast(%mp3,
  host="icecast",
  port=8000,
  password="hackme",
  mount="/stream.mp3",
  radio)
