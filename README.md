# MyKMoments
Store all your precious moments at one place.

# Tech stack
* Front-end: Svelte.js powered by [`create-svelte`](https://github.com/sveltejs/kit/tree/master/packages/create-svelte).
* Back-end: C++ 20
* Database: PostgreSQL

# Dependencies
## Front end
* Svelte.js
* Svelte Kit
## Back end
* [Crow](https://github.com/CrowCpp/Crow/) - C++ framework for web app/service
* OpenSSL - for HTTPS
* ZLib - for compression
* Standalone ASIO - required by Crow
* libpq - C library for accessing Postgres databases
* [libpqxx](https://github.com/jtv/libpqxx) - C++ wrapper library for libpq (Postgres)
* [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) - for JSON Web Tokens

# Cloning repo
```
git clone --recurse-submodules https://github.com/kiner-shah/mykmoments.git
git submodule update --init --depth 3 --recursive --remote --jobs 4
```

# Installating some pre-requisites
```
sudo apt-get update

sudo apt-get install libpq-dev libssl-dev zlib1g-dev

# For installing postgresql server
sudo apt-get install postgresql postgresql-contrib
```

# Note about Crow
You need to apply a [patch](crow_hack.patch) after cloning Crow because there is one [open issue](https://github.com/CrowCpp/Crow/issues/538) because of which Crow doesn't send proper response for preflight OPTIONS requests.

**UPDATE**: Crow has released new versions since then and the issue above is now fixed, so the patch may not be needed anymore.

# Database schema
Database schema is present [here](restapi/mkm_db.sql).

# Building and running REST API
```
cd restapi
mkdir build && cd build
cmake ..
make
./MyKMomentsRestAPI
```

# Running front end
```
npm install
npm run dev
```
