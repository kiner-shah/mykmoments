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


--
-- Name: fn_trig_moment_id(); Type: FUNCTION; Schema: public; Owner: mkm_user
--

CREATE FUNCTION public.fn_trig_moment_id() RETURNS trigger
    LANGUAGE plpgsql
    AS $$ 
begin
new.id = (select count(*) + 1 from moments where username = new.username);
return new;
end;
$$;


ALTER FUNCTION public.fn_trig_moment_id() OWNER TO mkm_user;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: moments; Type: TABLE; Schema: public; Owner: mkm_user
--

CREATE TABLE public.moments (
    username character varying(40) NOT NULL,
    title character varying(100) NOT NULL,
    description character varying(2000) NOT NULL,
    moment_date date NOT NULL,
    image_filename character varying(40),
    image_caption character varying(100),
    feelings character varying(20)[],
    image_data bytea,
    id bigint NOT NULL,
    created_date timestamp with time zone DEFAULT now() NOT NULL,
    last_modified_date timestamp with time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.moments OWNER TO mkm_user;

--
-- Name: users; Type: TABLE; Schema: public; Owner: mkm_user
--

CREATE TABLE public.users (
    username character varying(40) NOT NULL,
    fullname character varying(100) NOT NULL,
    birthdate date NOT NULL,
    emailid character varying(100) NOT NULL,
    password_hash character varying(100) NOT NULL,
    account_creation_time timestamp with time zone NOT NULL
);


ALTER TABLE public.users OWNER TO mkm_user;

--
-- Name: moments moments_pkey; Type: CONSTRAINT; Schema: public; Owner: mkm_user
--

ALTER TABLE ONLY public.moments
    ADD CONSTRAINT moments_pkey PRIMARY KEY (id, username);


--
-- Name: users unique_username; Type: CONSTRAINT; Schema: public; Owner: mkm_user
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT unique_username UNIQUE (username);


--
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: mkm_user
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (emailid);


--
-- Name: moments trig_moment_id; Type: TRIGGER; Schema: public; Owner: mkm_user
--

CREATE TRIGGER trig_moment_id BEFORE INSERT ON public.moments FOR EACH ROW EXECUTE FUNCTION public.fn_trig_moment_id();


--
-- Name: moments moments_username_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mkm_user
--

ALTER TABLE ONLY public.moments
    ADD CONSTRAINT moments_username_fkey FOREIGN KEY (username) REFERENCES public.users(username);


--
-- PostgreSQL database dump complete
--

