ARG PG_MAJOR=17
FROM postgres:$PG_MAJOR
ARG PG_MAJOR

RUN apt-get update && \
		apt-mark hold locales && \
		apt-get install -y --no-install-recommends build-essential postgresql-server-dev-$PG_MAJOR

COPY ./pgvector /tmp/pgvector

COPY . /tmp/pgsurv

COPY init.sql /docker-entrypoint-initdb.d/init.sql

RUN cd /tmp/pgsurv && \
		make clean && \
		make OPTFLAGS="" && \
		make install && \
		rm -r /tmp/pgsurv && \
		cd /tmp/pgvector && \
		#apt-get remove -y build-essential postgresql-server-dev-$PG_MAJOR && \
		apt-get autoremove -y && \
		#apt-mark unhold locales && \
		rm -rf /var/lib/apt/lists/*

RUN mkdir /data && \
		chown postgres:postgres data

COPY ./database_setup.sh /

RUN chmod 744 /database_setup.sh && \
		chown postgres:postgres database_setup.sh

# RUN su postgres

# RUN bash database_setup.sh

# CMD ["bash"]