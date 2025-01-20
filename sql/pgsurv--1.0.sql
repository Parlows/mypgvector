\echo Use "CREATE EXTENSION pgsurv" to load this file. \quit

-- Vector types
CREATE TYPE vector128;

CREATE FUNCTION vector128_in(cstring)
    RETURNS vector128
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector128_out(vector128)
    RETURNS cstring
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector128_recv(internal)
    RETURNS vector128
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector128_send(vector128)
    RETURNS bytea
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE vector128 (
    INTERNALLENGTH = 516,
	INPUT     = vector128_in,
	OUTPUT    = vector128_out,
	RECEIVE   = vector128_recv,
	SEND      = vector128_send
);

CREATE TYPE vector256;

CREATE FUNCTION vector256_in(cstring)
    RETURNS vector256
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector256_out(vector256)
    RETURNS cstring
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector256_recv(internal)
    RETURNS vector256
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector256_send(vector256)
    RETURNS bytea
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE vector256 (
    INTERNALLENGTH = 1028,
	INPUT     = vector256_in,
	OUTPUT    = vector256_out,
	RECEIVE   = vector256_recv,
	SEND      = vector256_send
);

CREATE TYPE vector512;

CREATE FUNCTION vector512_in(cstring)
    RETURNS vector512
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector512_out(vector512)
    RETURNS cstring
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector512_recv(internal)
    RETURNS vector512
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector512_send(vector512)
    RETURNS bytea
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE vector512 (
    INTERNALLENGTH = 2052,
	INPUT     = vector512_in,
	OUTPUT    = vector512_out,
	RECEIVE   = vector512_recv,
	SEND      = vector512_send
);

CREATE TYPE vector1024;

CREATE FUNCTION vector1024_in(cstring)
    RETURNS vector1024
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector1024_out(vector1024)
    RETURNS cstring
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector1024_recv(internal)
    RETURNS vector1024
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector1024_send(vector1024)
    RETURNS bytea
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE vector1024 (
    INTERNALLENGTH = 4100,
	INPUT     = vector1024_in,
	OUTPUT    = vector1024_out,
	RECEIVE   = vector1024_recv,
	SEND      = vector1024_send
);

CREATE TYPE vector2048;

CREATE FUNCTION vector2048_in(cstring)
    RETURNS vector2048
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector2048_out(vector2048)
    RETURNS cstring
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector2048_recv(internal)
    RETURNS vector2048
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION vector2048_send(vector2048)
    RETURNS bytea
    AS 'MODULE_PATHNAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE vector2048 (
    INTERNALLENGTH = 8196,
	INPUT     = vector2048_in,
	OUTPUT    = vector2048_out,
	RECEIVE   = vector2048_recv,
	SEND      = vector2048_send
);

-- Operators
CREATE FUNCTION cosine_distance128(vector128, vector128)
    RETURNS float8
    AS 'MODULE_PATHNAME', 'cosine_distance128'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cosine_distance256(vector256, vector256)
    RETURNS float8
    AS 'MODULE_PATHNAME', 'cosine_distance256'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cosine_distance512(vector512, vector512)
    RETURNS float8
    AS 'MODULE_PATHNAME', 'cosine_distance512'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cosine_distance1024(vector1024, vector1024)
    RETURNS float8
    AS 'MODULE_PATHNAME', 'cosine_distance1024'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cosine_distance2048(vector2048, vector2048)
    RETURNS float8
    AS 'MODULE_PATHNAME', 'cosine_distance2048'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <=> (
    LEFTARG = vector128,
    RIGHTARG = vector128,
    FUNCTION = cosine_distance128,
    COMMUTATOR = '<=>'
);

CREATE OPERATOR <=> (
    LEFTARG = vector256,
    RIGHTARG = vector256,
    FUNCTION = cosine_distance256,
    COMMUTATOR = '<=>'
);

CREATE OPERATOR <=> (
    LEFTARG = vector512,
    RIGHTARG = vector512,
    FUNCTION = cosine_distance512,
    COMMUTATOR = '<=>'
);

CREATE OPERATOR <=> (
    LEFTARG = vector1024,
    RIGHTARG = vector1024,
    FUNCTION = cosine_distance1024,
    COMMUTATOR = '<=>'
);

CREATE OPERATOR <=> (
    LEFTARG = vector2048,
    RIGHTARG = vector2048,
    FUNCTION = cosine_distance2048,
    COMMUTATOR = '<=>'
);
