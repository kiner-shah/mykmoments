--
-- PostgreSQL database dump
--

-- Dumped from database version 14.7 (Ubuntu 14.7-0ubuntu0.22.04.1)
-- Dumped by pg_dump version 14.7 (Ubuntu 14.7-0ubuntu0.22.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: pgcrypto; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS pgcrypto WITH SCHEMA public;


--
-- Name: EXTENSION pgcrypto; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION pgcrypto IS 'cryptographic functions';


SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: users; Type: TABLE; Schema: public; Owner: mkm_user
--

CREATE TABLE public.users (
    username character varying(40) NOT NULL,
    fullname character varying(100),
    birthdate date,
    emailid character varying(100),
    password_hash character varying(100),
    account_creation_time timestamp with time zone
);


ALTER TABLE public.users OWNER TO mkm_user;

--
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: mkm_user
--

COPY public.users (username, fullname, birthdate, emailid, password_hash, account_creation_time) FROM stdin;
user1	MKM User1	1999-02-02	user1@mkm.domain	$2a$08$Pk.4oxCHu3D8NdNeXLGZUexRx.y2S3cgZ6HI9rS8nqLq6xzpFP7em	2023-06-28 11:48:50.524399+05:30
kshah	Kiner Shah	1996-05-20	kiner.shah1@gmail.com	$2a$08$d8yaQH0Pu7JWqpUZf.vJGOZPi/EZyWO.muwChcirzEAqdjrLMfg32	2023-06-28 15:12:17.689539+05:30
\.


--
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: mkm_user
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (username);


--
-- PostgreSQL database dump complete
--

