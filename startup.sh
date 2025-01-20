docker stop postgres
docker rm postgres
docker image rm pgsurv
docker build . -t pgsurv
docker run -it --name postgres -e PGDATA=/data/ -e POSTGRES_HOST_AUTH_METHOD=trust -v /media/data/pg/:/data -p 5432:5432 pgsurv