# Ubuntu/Debian
sudo apt update
sudo apt install icecast2 liquidsoap

mkdir build
cd build
cmake ..
make
sudo make install


docker-compose logs -f icecast
docker-compose logs -f liquidsoap
docker-compose logs -f matrix-daemon
docker-compose logs -f aura-api

docker-compose build
docker-compose up -d

         +----------------+
         |  DJ / Live Mic |
         +--------+-------+
                  |
                  v
           +------+-------+
           | Icecast / HLS |
           +------+-------+
                  |
        ---------------------
        |                   |
+-------v-------+   +-------v-------+
| Web Player    |   | Mobile Player |
+---------------+   +---------------+

        +----------------+
        | Aura.radio API |
        +--------+-------+
                 ^
                 |
          +------v------+
          | qmatrixclient|
          |  C++ Daemon |
          +-------------+
       (Matrix chat control)

# Clone your qmatrixclient repo (or use your source folder)
git clone <https://github.com/quotient-im/libQuotient.git> qmatrixclient
cd libQuotient

git clone <https://github.com/matrix-org/matrix.to.git> qmatrixclient
cd matrix.to

git clone https://github.com/jothepro/doxygen-awesome-css.git
cd doxygen-awesome-css

# Create a build folder
mkdir build && cd build

# Configure with cmake
cmake ..

# Compile
make -j$(nproc)

# Optional: Install system-wide
sudo make install

# create and configure 
auraradio-docker/
├─ docker-compose.yml
├─ aura-api/           # Aura.radio backend
│   └─ ... (Python/FastAPI code)
├─ liquidsoap/         # Liquidsoap config & playlist
│   ├─ radio.liq
│   └─ playlist.m3u
├─ icecast/
│   └─ icecast.xml
├─ matrix-daemon/      # C++ qmatrixclient daemon
│   ├─ CMakeLists.txt
│   └─ daemon.cpp
└─ web/
    └─ index.html      # HTML5 player
