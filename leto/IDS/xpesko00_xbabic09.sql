--vymazanie tabuliek pred spustanim skriptu
DROP TABLE pivo CASCADE CONSTRAINT;
DROP TABLE recept CASCADE CONSTRAINT;
DROP TABLE slad CASCADE CONSTRAINT;
DROP TABLE chmel CASCADE CONSTRAINT;
DROP TABLE kvasnice CASCADE CONSTRAINT;
DROP TABLE uzivatel CASCADE CONSTRAINT;
DROP TABLE sladek CASCADE CONSTRAINT;
DROP TABLE diplom CASCADE CONSTRAINT;
DROP TABLE kamenna_predajna CASCADE CONSTRAINT;
DROP TABLE certifikat CASCADE CONSTRAINT;
DROP TABLE hospoda CASCADE CONSTRAINT;
DROP TABLE pivovar CASCADE CONSTRAINT;
DROP TABLE zmluva CASCADE CONSTRAINT;
DROP TABLE podrobnosti_kp CASCADE CONSTRAINT;
DROP TABLE podrobnosti_h CASCADE CONSTRAINT;
DROP TABLE hodnotenie_hospoda CASCADE CONSTRAINT;
DROP TABLE hodnotenie_pivo CASCADE CONSTRAINT;
DROP TABLE sledovanie_pivovar CASCADE CONSTRAINT;
DROP TABLE vyhladavanie_hospoda CASCADE CONSTRAINT;
DROP TABLE vyhladavanie_pivo CASCADE CONSTRAINT;
DROP TABLE vyhladavanie_pivovar CASCADE CONSTRAINT;
DROP TABLE pivo_slad CASCADE CONSTRAINT;
DROP TABLE pivo_chmel CASCADE CONSTRAINT;
DROP TABLE pivo_kvasnice CASCADE CONSTRAINT;
DROP TABLE kp_slad CASCADE CONSTRAINT;
DROP TABLE kp_chmel CASCADE CONSTRAINT;
DROP TABLE kp_kvasnice CASCADE CONSTRAINT;

DROP SEQUENCE recept_seq;

DROP PROCEDURE uroven_predajni;
DROP PROCEDURE podvodnici;

--vytvaranie tabuliek podla zadania projektu
CREATE TABLE pivo (
    PivoID NUMBER NOT NULL PRIMARY KEY,
    Nazov VARCHAR(255) NOT NULL,
    Farba_EBC NUMBER,
    Styl_kvasenia VARCHAR(255),
    Typ VARCHAR(255),
    Obsah_alkoholu NUMBER(2, 1) CHECK (Obsah_alkoholu<=50),
    Obdobie_varenia VARCHAR(255),

    varil_sladek NUMBER NOT NULL,
    varil_pivovar NUMBER NOT NULL
);

CREATE TABLE recept (
    ReceptID NUMBER NOT NULL PRIMARY KEY,
    Datum_pridania DATE,

    vydal_sladek NUMBER NOT NULL,
    opisuje_pivo NUMBER NOT NULL
);

CREATE TABLE slad (
    SladID NUMBER NOT NULL PRIMARY KEY,
    Farba VARCHAR(255),
    Povod VARCHAR(255),
    Extrakt VARCHAR(255)
);

CREATE TABLE chmel (
    ChmelID NUMBER NOT NULL PRIMARY KEY,
    Aroma VARCHAR(255),
    Horkost_IBU NUMBER(3),
    Podiel_kyselin NUMBER(3, 2),
    Povod VARCHAR(255),
    Doba_zberu VARCHAR(255)
);

CREATE TABLE kvasnice (
    KvasniceID NUMBER NOT NULL PRIMARY KEY,
    Skupenstvo VARCHAR(255),
    Styl_kvasenia VARCHAR(255)
);

CREATE TABLE uzivatel (
    UzivatelID NUMBER NOT NULL PRIMARY KEY,
    Meno VARCHAR(255) NOT NULL,
    Mnozstvo_vypiteho_piva NUMBER
);

CREATE TABLE sladek (
    SladekID NUMBER NOT NULL PRIMARY KEY,
    Generacia NUMBER,
    Druh VARCHAR(255),

    sladkovsky_diplom NUMBER NOT NULL,
    uzivatelske_konto NUMBER NOT NULL
);

CREATE TABLE diplom (
    DiplomID NUMBER NOT NULL PRIMARY KEY,
    Notar VARCHAR(255)
);

CREATE TABLE kamenna_predajna (
    KpID NUMBER NOT NULL PRIMARY KEY,
    Nazov VARCHAR(255) NOT NULL,

    Mesto VARCHAR(255),
    Ulica VARCHAR(255),
    Cislo_ulice VARCHAR(255),
    Krajina VARCHAR(255)
);

CREATE TABLE certifikat (
    CertifikatID NUMBER NOT NULL PRIMARY KEY,

    Platnost_do DATE,
    Datum_vlozenia DATE,

    drzitel NUMBER NOT NULL
);

CREATE TABLE hospoda (
    HospodaID NUMBER NOT NULL PRIMARY KEY,
    Nazov VARCHAR(255) NOT NULL,

    Mesto VARCHAR(255),
    Ulica VARCHAR(255),
    Cislo_ulice VARCHAR(255),
    Krajina VARCHAR(255)
);

CREATE TABLE pivovar (
    PivovarID NUMBER NOT NULL PRIMARY KEY,
    Nazov VARCHAR(255) NOT NULL,

    Mesto VARCHAR(255),
    Ulica VARCHAR(255),
    Cislo_ulice VARCHAR(255),
    Krajina VARCHAR(255),

    Znacka VARCHAR(255) NOT NULL,
    Rok_zalozenia NUMBER
);

CREATE TABLE zmluva (
    ZmluvaID NUMBER NOT NULL PRIMARY KEY,
    Platnost_zaciatok DATE,
    Platnost_koniec DATE,
    Zlava NUMBER,

    Hospoda_id NUMBER NOT NULL,
    Pivovar_id NUMBER NOT NULL
);

CREATE TABLE podrobnosti_kp (
    Mnozstvo NUMBER,
    Distribucia VARCHAR(255),

    kp NUMBER NOT NULL,
    o_pive NUMBER NOT NULL,

    CONSTRAINT PK_podrobnosti_kp PRIMARY KEY (kp, o_pive)
);

CREATE TABLE podrobnosti_h (
    Mnozstvo NUMBER,
    Distribucia VARCHAR(255),

    Dostupne_pivo NUMBER NOT NULL,
    PK_hospody NUMBER NOT NULL,

    CONSTRAINT PK_podrobnosti_h PRIMARY KEY (Dostupne_pivo, PK_hospody)
);

CREATE TABLE hodnotenie_hospoda (
    Datum DATE,
    Pocet_hviezdiciek NUMBER(1),

    hodnotena_hospoda NUMBER NOT NULL,
    hodnotiaci_uzivatel NUMBER NOT NULL,

    CONSTRAINT PK_hodnotenie_hospoda PRIMARY KEY (hodnotena_hospoda, hodnotiaci_uzivatel)
);

CREATE TABLE hodnotenie_pivo (
    Datum DATE,
    Pocet_hviezdiciek NUMBER(1),

    hodnotene_pivo NUMBER NOT NULL,
    hodnotiaci_uzivatel NUMBER NOT NULL,

    CONSTRAINT PK_hodnotenie_pivo PRIMARY KEY (hodnotene_pivo, hodnotiaci_uzivatel)
);

CREATE TABLE sledovanie_pivovar (
    uzivatel_id NUMBER NOT NULL,
    pivovar_id NUMBER NOT NULL,

    CONSTRAINT uzivatel_sleduje FOREIGN KEY(uzivatel_id) REFERENCES uzivatel(UzivatelID),
    CONSTRAINT pivovar_je_sledovany FOREIGN KEY(pivovar_id) REFERENCES pivovar(PivovarID),

    CONSTRAINT PK_sledovanie_pivovar PRIMARY KEY (uzivatel_id, pivovar_id)
);

CREATE TABLE vyhladavanie_hospoda (
    uzivatel_hospoda_id NUMBER NOT NULL,
    vyhladana_hospoda_id NUMBER NOT NULL,
    datum_vyhladavania DATE,

    CONSTRAINT uzivatel_vyhladava_hospodu FOREIGN KEY(uzivatel_hospoda_id) REFERENCES uzivatel(UzivatelID),
    CONSTRAINT hospoda_je_hladana FOREIGN KEY(vyhladana_hospoda_id) REFERENCES hospoda(HospodaID),

    CONSTRAINT PK_vyhladavanie_hospoda PRIMARY KEY (uzivatel_hospoda_id, vyhladana_hospoda_id)
);

CREATE TABLE vyhladavanie_pivovar (
    uzivatel_id NUMBER NOT NULL,
    pivovar_id NUMBER NOT NULL,
    datum_vyhladavania DATE,

    CONSTRAINT uzivatel_vyhladava_pivovar FOREIGN KEY(uzivatel_id) REFERENCES uzivatel(UzivatelID),
    CONSTRAINT pivovar_je_hladany FOREIGN KEY(pivovar_id) REFERENCES pivovar(PivovarID),

    CONSTRAINT PK_vyhladavanie_pivovar PRIMARY KEY (uzivatel_id, pivovar_id)
);

CREATE TABLE vyhladavanie_pivo (
    uzivatel_id NUMBER NOT NULL,
    pivo_id NUMBER NOT NULL,
    datum_vyhladavania DATE,

    CONSTRAINT uzivatel_vyhladava_pivo FOREIGN KEY(uzivatel_id) REFERENCES uzivatel(UzivatelID),
    CONSTRAINT pivo_je_hladane FOREIGN KEY(pivo_id) REFERENCES pivo(PivoID),

    CONSTRAINT PK_vyhladavanie_pivo PRIMARY KEY (uzivatel_id, pivo_id)
);

CREATE TABLE pivo_slad (
    Pivo_FK NUMBER NOT NULL,
    Slad_FK NUMBER NOT NULL,

    CONSTRAINT slad_na_pivo FOREIGN KEY(Pivo_FK) REFERENCES pivo(PivoID),
    CONSTRAINT slad_v_pive FOREIGN KEY(Slad_FK) REFERENCES slad(SladID),

    CONSTRAINT PK_pivo_slad PRIMARY KEY (Pivo_FK, Slad_FK)
);

CREATE TABLE pivo_chmel (
    Pivo_FK NUMBER NOT NULL,
    Chmel_FK NUMBER NOT NULL,

    CONSTRAINT chmel_na_pivo FOREIGN KEY(Pivo_FK) REFERENCES pivo(PivoID),
    CONSTRAINT chmel_v_pive FOREIGN KEY(Chmel_FK) REFERENCES chmel(ChmelID),

    CONSTRAINT PK_pivo_chmel PRIMARY KEY (Pivo_FK, Chmel_FK)
);

CREATE TABLE pivo_kvasnice (
    Pivo_FK NUMBER NOT NULL,
    Kvasnice_FK NUMBER NOT NULL,

    CONSTRAINT kvasnice_na_pivo FOREIGN KEY(Pivo_FK) REFERENCES pivo(PivoID),
    CONSTRAINT kvasnice_v_pive FOREIGN KEY(Kvasnice_FK) REFERENCES kvasnice(KvasniceID),

    CONSTRAINT PK_pivo_kvasnice PRIMARY KEY (Pivo_FK, Kvasnice_FK)
);

CREATE TABLE kp_slad (
    KP_FK NUMBER NOT NULL,
    Slad_FK NUMBER NOT NULL,

    CONSTRAINT predajna_sladu FOREIGN KEY(KP_FK) REFERENCES kamenna_predajna(KpID),
    CONSTRAINT slad_v_predajni FOREIGN KEY(Slad_FK) REFERENCES slad(SladID),

    CONSTRAINT PK PRIMARY KEY (KP_FK, Slad_FK)
);

CREATE TABLE kp_chmel (
    KP_FK NUMBER NOT NULL,
    Chmel_FK NUMBER NOT NULL,

    CONSTRAINT predajna_chmelu FOREIGN KEY(KP_FK) REFERENCES kamenna_predajna(KpID),
    CONSTRAINT chmel_v_predajni FOREIGN KEY(Chmel_FK) REFERENCES chmel(ChmelID),

    CONSTRAINT PK_kp_chmel PRIMARY KEY (KP_FK, Chmel_FK)
);

CREATE TABLE kp_kvasnice (
    KP_FK NUMBER NOT NULL,
    Kvasnice_FK NUMBER NOT NULL,

    CONSTRAINT predajna_kvasnic FOREIGN KEY(KP_FK) REFERENCES kamenna_predajna(KpID),
    CONSTRAINT kvasnice_v_predajni FOREIGN KEY(Kvasnice_FK) REFERENCES kvasnice(KvasniceID),

    CONSTRAINT PK_kp_kvasnice PRIMARY KEY (KP_FK, Kvasnice_FK)
);

--previazanie tabuliek, spajanie primarnych a cudzich klucov
ALTER TABLE pivo ADD CONSTRAINT varene_sladkom FOREIGN KEY(varil_sladek) REFERENCES sladek;
ALTER TABLE pivo ADD CONSTRAINT varene_pivovarom FOREIGN KEY(varil_pivovar) REFERENCES pivovar;

ALTER TABLE recept ADD CONSTRAINT vydane_sladkom FOREIGN KEY(vydal_sladek) REFERENCES sladek;
ALTER TABLE recept ADD CONSTRAINT na_pivo FOREIGN KEY(opisuje_pivo) REFERENCES pivo;

ALTER TABLE sladek ADD CONSTRAINT ma_diplom FOREIGN KEY(sladkovsky_diplom) REFERENCES diplom;
ALTER TABLE sladek ADD CONSTRAINT je_uzivatel FOREIGN KEY(uzivatelske_konto) REFERENCES uzivatel;

ALTER TABLE podrobnosti_kp ADD CONSTRAINT opisuje_kp FOREIGN KEY(kp) REFERENCES kamenna_predajna;
ALTER TABLE podrobnosti_kp ADD CONSTRAINT opisuje_pivo FOREIGN KEY(o_pive) REFERENCES pivo;

ALTER TABLE certifikat ADD CONSTRAINT certifikuje_predajnu FOREIGN KEY(drzitel) REFERENCES kamenna_predajna;

ALTER TABLE podrobnosti_h ADD CONSTRAINT pivo_v_hospode FOREIGN KEY(Dostupne_pivo) REFERENCES pivo;
ALTER TABLE podrobnosti_h ADD CONSTRAINT id_hospody FOREIGN KEY(PK_hospody) REFERENCES hospoda;

ALTER TABLE hodnotenie_pivo ADD CONSTRAINT pivo_na_hodnotenie FOREIGN KEY(hodnotene_pivo) REFERENCES pivo;
ALTER TABLE hodnotenie_pivo ADD CONSTRAINT hodnotenie_uzivatela FOREIGN KEY(hodnotiaci_uzivatel) REFERENCES uzivatel;

ALTER TABLE hodnotenie_hospoda ADD CONSTRAINT hospoda_na_hodnotenie FOREIGN KEY(hodnotena_hospoda) REFERENCES hospoda;
ALTER TABLE hodnotenie_hospoda ADD CONSTRAINT id_uzivatela FOREIGN KEY(hodnotiaci_uzivatel) REFERENCES uzivatel;

ALTER TABLE zmluva ADD CONSTRAINT s_hospodou FOREIGN KEY(Hospoda_id) REFERENCES hospoda;
ALTER TABLE zmluva ADD CONSTRAINT s_pivovarom FOREIGN KEY(Pivovar_id) REFERENCES pivovar;

--triggery
CREATE SEQUENCE recept_seq
    START WITH 1
    INCREMENT BY 1;

CREATE OR REPLACE TRIGGER recept_pk
    BEFORE INSERT ON recept
    FOR EACH ROW
BEGIN
    :NEW.ReceptID := recept_seq.nextval;
END;
/

CREATE OR REPLACE TRIGGER pivo_farba_kvasenie
    BEFORE INSERT OR UPDATE ON pivo
    FOR EACH ROW
BEGIN
    IF NOT (:NEW.Farba_EBC IN ('4', '6', '8', '12', '16', '20', '26', '33', '39', '47', '57', '69', '79'))
        THEN RAISE_APPLICATION_ERROR(-20000, 'Nespravne zadana farba v stupnici EBC!');
    END IF;

    IF NOT (:NEW.Styl_kvasenia IN ('spodni', 'svrchni', 'spontanni'))
        THEN RAISE_APPLICATION_ERROR(-20001, 'Nespravne zadany styl kvasenia! svrchni|spodni|spontanni');
    END IF;
END;
/

--procedury
CREATE OR REPLACE PROCEDURE podvodnici AS
    CURSOR zmluvy IS SELECT * FROM zmluva;
    pocet_podvodnikov INTEGER;
    akt_zmluva zmluvy%ROWTYPE;

    BEGIN
        pocet_podvodnikov := 0;
        OPEN zmluvy;

        LOOP
            FETCH zmluvy INTO akt_zmluva;
            EXIT WHEN zmluvy%NOTFOUND;

            IF NOT (akt_zmluva.Zlava BETWEEN 0 AND 90)
                THEN DBMS_OUTPUT.put_line('Zmluva ' || akt_zmluva.ZmluvaID || ' je podozriva z podvodu!');
                pocet_podvodnikov := pocet_podvodnikov + 1;
            END IF;
        END LOOP;
        DBMS_OUTPUT.put_line('Celkovy pocet podozrivych zmluv: ' || pocet_podvodnikov);
        CLOSE zmluvy;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RAISE_APPLICATION_ERROR(-20100, 'Chyba pocas prevadzania operacie SELECT!');
        WHEN OTHERS THEN
            RAISE_APPLICATION_ERROR(-20009, 'Neocakavana chyba pocas procedury podvodnici!');
    END;
/

CREATE OR REPLACE PROCEDURE uroven_predajni AS
    CURSOR predajne IS SELECT * FROM kamenna_predajna;
    CURSOR certifikaty IS SELECT * FROM certifikat;
    pocet_certifikatov INTEGER;

    akt_predajna predajne%ROWTYPE;
    akt_certifikat certifikaty%ROWTYPE;

    BEGIN
        OPEN predajne;

        LOOP
            OPEN certifikaty;
            FETCH predajne INTO akt_predajna;
            EXIT WHEN predajne%NOTFOUND;

            pocet_certifikatov := 0;

            LOOP
                FETCH certifikaty INTO akt_certifikat;
                EXIT WHEN certifikaty%NOTFOUND;

                IF (akt_predajna.KpID = akt_certifikat.drzitel)
                    THEN pocet_certifikatov := pocet_certifikatov + 1;
                END IF;
            END LOOP;

            DBMS_OUTPUT.put_line('Kamenna predajna ' || akt_predajna.Nazov || ' je certifikovana ' || pocet_certifikatov || ' krat');
            CLOSE certifikaty;
        END LOOP;
        CLOSE predajne;

    EXCEPTION
        WHEN OTHERS THEN
            RAISE_APPLICATION_ERROR(-20010, 'Neocakavana chyba pocas procedury uroven_predajni!');
    END;
/


--vkladanie hodnot do tabuliek
INSERT INTO uzivatel(UzivatelID, Meno, Mnozstvo_vypiteho_piva)
    VALUES ('001', 'Jozko Mrkvicka', '32');
INSERT INTO uzivatel(UzivatelID, Meno, Mnozstvo_vypiteho_piva)
    VALUES ('002', 'Anicka Mrkvickova', '88');
INSERT INTO uzivatel(UzivatelID, Meno, Mnozstvo_vypiteho_piva)
    VALUES ('003', 'Palko Mrkvicka', '666');

INSERT INTO diplom(DiplomID, Notar)
    VALUES ('001', 'ujo Fero');
INSERT INTO diplom(DiplomID, Notar)
    VALUES ('002', 'ujo Jozo');
INSERT INTO diplom(DiplomID, Notar)
    VALUES ('003', 'teta Erika');

INSERT INTO sladek(SladekID, Generacia, Druh, sladkovsky_diplom, uzivatelske_konto)
    VALUES ('001', '3', 'na volnej nohe', '001', '003');
INSERT INTO sladek(SladekID, Generacia, Druh, sladkovsky_diplom, uzivatelske_konto)
    VALUES ('002', '2', 'pre pivovar', '002', '002');
INSERT INTO sladek(SladekID, Generacia, Druh, sladkovsky_diplom, uzivatelske_konto)
    VALUES ('003', '3', 'aj aj', '003', '003');

INSERT INTO pivovar(PivovarID, Nazov, Mesto, Ulica, Cislo_ulice, Krajina, Znacka, Rok_zalozenia)
    VALUES ('001', 'Varime nie len pivo', 'Bratislava', 'Dubravka', '12', 'Slovensko', 'Mercedes', '1998');
INSERT INTO pivovar(PivovarID, Nazov, Mesto, Ulica, Cislo_ulice, Krajina, Znacka, Rok_zalozenia)
    VALUES ('002', 'Vase promile, nasa radost', 'In the middle of nowhere', 'Nowhere', '00', 'Unknown', 'BMW', '2013');
INSERT INTO pivovar(PivovarID, Nazov, Mesto, Ulica, Cislo_ulice, Krajina, Znacka, Rok_zalozenia)
    VALUES ('003', 'Varime nie len pivo', 'Bratislava', 'Dubravka', '12', 'Slovensko', 'Skoda', '1234');

INSERT INTO pivo(PivoID, Nazov, Farba_EBC, Styl_kvasenia, Typ, Obsah_alkoholu, Obdobie_varenia, varil_sladek, varil_pivovar)
    VALUES ('001', 'Radek', '8', 'spodni', 'nevhodny', '0.45', 'vcera', '001', '003');
INSERT INTO pivo(PivoID, Nazov, Farba_EBC, Styl_kvasenia, Typ, Obsah_alkoholu, Obdobie_varenia, varil_sladek, varil_pivovar)
    VALUES ('002', 'Plzen', '69', 'svrchni', 'vhodny', '0.69', 'zajtra', '002', '001');
INSERT INTO pivo(PivoID, Nazov, Farba_EBC, Styl_kvasenia, Obsah_alkoholu, Obdobie_varenia, varil_sladek, varil_pivovar)
    VALUES ('003', 'StaroCider', '33', 'spontanni', '0.42', 'cez leto', '001', '002');
--predvedenie triggeru pivo_farba_kvasenie - kontrola spravnosti zadanych udajov
INSERT INTO pivo(PivoID, Nazov, Farba_EBC, Styl_kvasenia, Typ, Obsah_alkoholu, Obdobie_varenia, varil_sladek, varil_pivovar)
    VALUES ('004', 'Squarepants', '80', 'zly', 'maly', '0.59', 'zimne', '003', '001');

--predvedenie triggeru recept_pk - automaticke insertovanie primarneho kluca
INSERT INTO recept(Datum_pridania, vydal_sladek, opisuje_pivo)
    VALUES ('03/JAN/2015', '001', '003');
INSERT INTO recept(Datum_pridania, vydal_sladek, opisuje_pivo)
    VALUES ('02/SEP/1954', '003', '001');
INSERT INTO recept(Datum_pridania, vydal_sladek, opisuje_pivo)
    VALUES ('15/SEP/2013', '001', '003');

SELECT * FROM recept;

INSERT INTO slad(SladID, Farba, Povod, Extrakt)
    VALUES ('001', 'cervena', 'slovensky', 'ziadny');
INSERT INTO slad(SladID, Farba, Povod, Extrakt)
    VALUES ('002', 'zlta', 'rusky', 'specialny');
INSERT INTO slad(SladID, Farba, Povod, Extrakt)
    VALUES ('003', 'zelena', 'americky', 'internetovy');

INSERT INTO chmel(ChmelID, Aroma, Horkost_IBU, Podiel_kyselin, Povod, Doba_zberu)
    VALUES ('001', 'salviova', '69', '0.23', 'cesko', 'leto');
INSERT INTO chmel(ChmelID, Aroma, Horkost_IBU, Podiel_kyselin, Povod, Doba_zberu)
    VALUES ('002', 'levandulova', '96', '0.25', 'ceskoslovensko', 'zima');
INSERT INTO chmel(ChmelID, Aroma, Horkost_IBU, Podiel_kyselin, Povod, Doba_zberu)
    VALUES ('003', 'matova', '42', '0.63', 'francuzsko', 'minuly rok');

INSERT INTO kvasnice(KvasniceID, Skupenstvo, Styl_kvasenia)
    VALUES ('001', 'ako nic', 'nudny');
INSERT INTO kvasnice(KvasniceID, Skupenstvo, Styl_kvasenia)
    VALUES ('002', 'plynne', 'spodny');
INSERT INTO kvasnice(KvasniceID, Skupenstvo, Styl_kvasenia)
    VALUES ('003', 'kvapalne', 'svrchny');

INSERT INTO kamenna_predajna(KpID, Nazov, Mesto, Ulica, Cislo_ulice, Krajina)
    VALUES ('001', 'U Havrana', 'Brno', 'Vlhka', '45', 'Africka republika');
INSERT INTO kamenna_predajna(KpID, Nazov, Mesto, Ulica, Cislo_ulice, Krajina)
    VALUES ('002', 'U Maciatok', 'Praha', 'Dlha', '46', 'Juhoafricka republika');
INSERT INTO kamenna_predajna(KpID, Nazov, Mesto, Ulica, Cislo_ulice, Krajina)
    VALUES ('003', 'Madagaskar Beer Bar', 'Bratislava', 'Prazska', '88', 'Madagaskar');

INSERT INTO certifikat(CertifikatID, Platnost_do, Datum_vlozenia, drzitel)
    VALUES ('001', '16/MAR/2013', '17/JUN/1998', '001');
INSERT INTO certifikat(CertifikatID, Platnost_do, Datum_vlozenia, drzitel)
    VALUES ('002', '21/JUL/2016', '07/AUG/1998', '002');
INSERT INTO certifikat(CertifikatID, Platnost_do, Datum_vlozenia, drzitel)
    VALUES ('003', '01/JUL/2003', '23/AUG/1995', '001');
INSERT INTO certifikat(CertifikatID, Platnost_do, Datum_vlozenia, drzitel)
    VALUES ('004', '08/AUG/2012', '15/MAR/1984', '003');

INSERT INTO hospoda(HospodaID, Nazov, Mesto, Ulica, Cislo_ulice, Krajina)
    VALUES ('001', 'U tvojej mamky', 'Brno', 'Babicova', '420', 'CR');
INSERT INTO hospoda(HospodaID, Nazov, Mesto, Ulica, Cislo_ulice, Krajina)
    VALUES ('002', 'U mojej mamky', 'Amsterdam', 'Peskova', '69', 'Holandsko');
INSERT INTO hospoda(HospodaID, Nazov, Mesto, Ulica, Cislo_ulice, Krajina)
    VALUES ('003', 'U smajdovej manky', 'Bratislava', 'Pikerska', '123', 'Slovensko');

INSERT INTO zmluva(ZmluvaID, Platnost_zaciatok, Platnost_koniec, Zlava, Hospoda_id, Pivovar_id)
    VALUES ('001', '12/DEC/2002', '23/NOV/2003', '12', '002', '002');
INSERT INTO zmluva(ZmluvaID, Platnost_zaciatok, Platnost_koniec, Zlava, Hospoda_id, Pivovar_id)
    VALUES ('002', '12/DEC/1999', '23/FEB/2013', '69', '002', '001');
INSERT INTO zmluva(ZmluvaID, Platnost_zaciatok, Platnost_koniec, Zlava, Hospoda_id, Pivovar_id)
    VALUES ('003', '02/FEB/2002', '02/FEB/2020', '02', '001', '003');

INSERT INTO podrobnosti_kp(Mnozstvo, Distribucia, kp, o_pive)
    VALUES ('12', 'Blachovka', '002', '001');
INSERT INTO podrobnosti_kp(Mnozstvo, Distribucia, kp, o_pive)
    VALUES ('42', 'Sud', '002', '003');
INSERT INTO podrobnosti_kp(Mnozstvo, Distribucia, kp, o_pive)
    VALUES ('69', 'Injekcna_striekacka', '003', '001');

INSERT INTO podrobnosti_h(Mnozstvo, Distribucia, Dostupne_pivo, PK_hospody)
    VALUES ('85', 'Kelimok', '001', '003');
INSERT INTO podrobnosti_h(Mnozstvo, Distribucia, Dostupne_pivo, PK_hospody)
    VALUES ('36', 'Sacok', '002', '003');
INSERT INTO podrobnosti_h(Mnozstvo, Distribucia, Dostupne_pivo, PK_hospody)
    VALUES ('43', 'Flasa', '003', '001');

INSERT INTO hodnotenie_hospoda(Datum, Pocet_hviezdiciek, hodnotena_hospoda, hodnotiaci_uzivatel)
    VALUES ('02/FEB/2020', '5', '001', '002');
INSERT INTO hodnotenie_hospoda(Datum, Pocet_hviezdiciek, hodnotena_hospoda, hodnotiaci_uzivatel)
    VALUES ('02/FEB/2020', '2', '002', '002');
INSERT INTO hodnotenie_hospoda(Datum, Pocet_hviezdiciek, hodnotena_hospoda, hodnotiaci_uzivatel)
    VALUES ('02/FEB/2020', '9', '003', '002');

INSERT INTO hodnotenie_pivo(Datum, Pocet_hviezdiciek, hodnotene_pivo, hodnotiaci_uzivatel)
    VALUES ('02/FEB/2020', '4', '001', '003');
INSERT INTO hodnotenie_pivo(Datum, Pocet_hviezdiciek, hodnotene_pivo, hodnotiaci_uzivatel)
    VALUES ('02/FEB/2020', '8', '001', '002');
INSERT INTO hodnotenie_pivo(Datum, Pocet_hviezdiciek, hodnotene_pivo, hodnotiaci_uzivatel)
    VALUES ('23/NOV/2003', '7', '002', '003');

INSERT INTO sledovanie_pivovar(uzivatel_id, pivovar_id)
    VALUES ('001', '003');
INSERT INTO sledovanie_pivovar(uzivatel_id, pivovar_id)
    VALUES ('002', '003');
INSERT INTO sledovanie_pivovar(uzivatel_id, pivovar_id)
    VALUES ('001', '002');

INSERT INTO vyhladavanie_hospoda(uzivatel_hospoda_id, vyhladana_hospoda_id, datum_vyhladavania)
    VALUES ('001', '003', '12/DEC/1999');
INSERT INTO vyhladavanie_hospoda(uzivatel_hospoda_id, vyhladana_hospoda_id, datum_vyhladavania)
    VALUES ('002', '001', '23/NOV/2003');
INSERT INTO vyhladavanie_hospoda(uzivatel_hospoda_id, vyhladana_hospoda_id, datum_vyhladavania)
    VALUES ('002', '003', '21/JUL/2016');

INSERT INTO vyhladavanie_pivovar(uzivatel_id, pivovar_id, datum_vyhladavania)
    VALUES ('002', '003', '21/JUL/2016');
INSERT INTO vyhladavanie_pivovar(uzivatel_id, pivovar_id, datum_vyhladavania)
    VALUES ('001', '001', '12/DEC/1999');
INSERT INTO vyhladavanie_pivovar(uzivatel_id, pivovar_id, datum_vyhladavania)
    VALUES ('003', '002', '23/NOV/2003');

INSERT INTO vyhladavanie_pivo(uzivatel_id, pivo_id, datum_vyhladavania)
    VALUES ('001', '003', '23/NOV/2003');
INSERT INTO vyhladavanie_pivo(uzivatel_id, pivo_id, datum_vyhladavania)
    VALUES ('001', '002', '21/JUL/2016');
INSERT INTO vyhladavanie_pivo(uzivatel_id, pivo_id, datum_vyhladavania)
    VALUES ('002', '001', '12/DEC/1999');

INSERT INTO pivo_slad(Pivo_FK, Slad_FK)
    VALUES ('001', '001');
INSERT INTO pivo_slad(Pivo_FK, Slad_FK)
    VALUES ('002', '002');
INSERT INTO pivo_slad(Pivo_FK, Slad_FK)
    VALUES ('003', '001');

INSERT INTO pivo_chmel(Pivo_FK, Chmel_FK)
    VALUES ('001', '002');
INSERT INTO pivo_chmel(Pivo_FK, Chmel_FK)
    VALUES ('002', '001');
INSERT INTO pivo_chmel(Pivo_FK, Chmel_FK)
    VALUES ('003', '002');

INSERT INTO pivo_kvasnice(Pivo_FK, Kvasnice_FK)
    VALUES ('001', '001');
INSERT INTO pivo_kvasnice(Pivo_FK, Kvasnice_FK)
    VALUES ('002', '003');
INSERT INTO pivo_kvasnice(Pivo_FK, Kvasnice_FK)
    VALUES ('003', '003');

INSERT INTO kp_chmel(KP_FK, Chmel_FK)
    VALUES ('001', '002');
INSERT INTO kp_chmel(KP_FK, Chmel_FK)
    VALUES ('002', '003');
INSERT INTO kp_chmel(KP_FK, Chmel_FK)
    VALUES ('001', '003');

INSERT INTO kp_kvasnice(KP_FK, Kvasnice_FK)
    VALUES ('003', '002');
INSERT INTO kp_kvasnice(KP_FK, Kvasnice_FK)
    VALUES ('002', '002');
INSERT INTO kp_kvasnice(KP_FK, Kvasnice_FK)
    VALUES ('003', '001');

INSERT INTO kp_slad(KP_FK, Slad_FK)
    VALUES ('003', '001');
INSERT INTO kp_slad(KP_FK, Slad_FK)
    VALUES ('003', '002');
INSERT INTO kp_slad(KP_FK, Slad_FK)
    VALUES ('001', '003');

--vypise informacie o receptoch, ktore vydal sladek typu 'na volnej nohe'
SELECT vydal_sladek, opisuje_pivo, generacia
FROM recept
INNER JOIN sladek
ON recept.vydal_sladek = sladek.SladekID
WHERE Druh='na volnej nohe';

--vypise informacie o pivovaroch, ktore varia pivo 'Radek'
SELECT pivovar.Nazov, Krajina, Obsah_alkoholu, varil_sladek
FROM pivovar
INNER JOIN pivo
ON pivovar.PivovarID = pivo.varil_pivovar
WHERE pivo.Nazov='Radek';

--vypise informacie o datume konca zmluvy medzi hospodami sidliacimi v Amsteradme a pivovarmi
SELECT hospoda.Nazov, pivovar.Nazov, zmluva.Platnost_koniec
FROM hospoda
INNER JOIN zmluva
ON hospoda.HospodaID = zmluva.Hospoda_id
INNER JOIN pivovar
ON zmluva.Pivovar_id = pivovar.PivovarID
WHERE hospoda.mesto = 'Amsterdam';

--vypise vsetky hodnotene piva od najlepsieho po najhorsie ohodnotene
SELECT pivo.Nazov, hodnotenie_pivo.Pocet_hviezdiciek, hodnotenie_pivo.hodnotiaci_uzivatel
FROM hodnotenie_pivo
INNER JOIN pivo
ON hodnotenie_pivo.hodnotene_pivo = pivo.PivoID
GROUP BY pivo.Nazov, hodnotenie_pivo.Pocet_hviezdiciek, hodnotenie_pivo.hodnotiaci_uzivatel
ORDER BY MAX(hodnotenie_pivo.Pocet_hviezdiciek) DESC;

--vypise pocet pivovarov v kazdej krajine
SELECT COUNT(PivovarID), Krajina
FROM pivovar
GROUP BY Krajina;

--vypise pivovary, ktore varia piva s obsahom alkoholu vyssim ako 0.5
SELECT Nazov
FROM pivovar
WHERE EXISTS(SELECT pivo.Nazov FROM pivo WHERE pivo.varil_pivovar = pivovar.PivovarID AND pivo.Obsah_alkoholu > 0.5);

--vypise informacie o kamennych predajniach, ktore sidlia v meste, kde sa nachadza aj pivovar
SELECT *
FROM kamenna_predajna
WHERE Mesto IN (SELECT Mesto FROM pivovar);

--SET SERVEROUTPUT ON;
--BEGIN
  --  podvodnici();
    --uroven_predajni();
--END;

--vytvorenie indexu a EXPLAIN PLAN
DROP INDEX index_zmluva_pivovar;

EXPLAIN PLAN FOR
    SELECT pivo.Nazov, hodnotenie_pivo.Pocet_hviezdiciek, hodnotenie_pivo.hodnotiaci_uzivatel
    FROM hodnotenie_pivo
    INNER JOIN pivo
    ON hodnotenie_pivo.hodnotene_pivo = pivo.PivoID
    GROUP BY pivo.Nazov, hodnotenie_pivo.Pocet_hviezdiciek, hodnotenie_pivo.hodnotiaci_uzivatel
    ORDER BY MAX(hodnotenie_pivo.Pocet_hviezdiciek) DESC;
SELECT plan_table_output FROM TABLE (dbms_xplan.display());

CREATE INDEX index_zmluva_pivovar ON hodnotenie_pivo(hodnotene_pivo);

EXPLAIN PLAN FOR
    SELECT pivo.Nazov, hodnotenie_pivo.Pocet_hviezdiciek, hodnotenie_pivo.hodnotiaci_uzivatel
    FROM hodnotenie_pivo
    INNER JOIN pivo
    ON hodnotenie_pivo.hodnotene_pivo = pivo.PivoID
    GROUP BY pivo.Nazov, hodnotenie_pivo.Pocet_hviezdiciek, hodnotenie_pivo.hodnotiaci_uzivatel
    ORDER BY MAX(hodnotenie_pivo.Pocet_hviezdiciek) DESC;
SELECT plan_table_output FROM TABLE (dbms_xplan.display());

--definicia pristupovych prav pre druheho clena teamu
GRANT ALL ON diplom TO xbabic09;
GRANT ALL ON kamenna_predajna TO xbabic09;
GRANT SELECT, UPDATE ON certifikat TO xbabic09;
GRANT ALL ON hospoda TO xbabic09;
GRANT ALL ON pivovar TO xbabic09;
GRANT SELECT, UPDATE ON zmluva TO xbabic09;
GRANT ALL ON podrobnosti_kp TO xbabic09;
GRANT ALL ON podrobnosti_h TO xbabic09;
GRANT SELECT, UPDATE ON hodnotenie_hospoda TO xbabic09;
GRANT SELECT, UPDATE ON hodnotenie_pivo TO xbabic09;
GRANT INSERT, UPDATE ON sledovanie_pivovar TO xbabic09;
GRANT UPDATE ON vyhladavanie_hospoda TO xbabic09;
GRANT UPDATE ON vyhladavanie_pivo TO xbabic09;
GRANT UPDATE ON vyhladavanie_pivovar TO xbabic09;
GRANT ALL ON pivo_slad TO xbabic09;
GRANT ALL ON pivo_chmel TO xbabic09;
GRANT ALL ON pivo_kvasnice TO xbabic09;
GRANT INSERT ON kp_slad TO xbabic09;
GRANT INSERT ON kp_chmel TO xbabic09;
GRANT INSERT ON kp_kvasnice TO xbabic09;
GRANT ALL ON pivo TO xbabic09;
GRANT INSERT, UPDATE ON slad TO xbabic09;
GRANT INSERT, UPDATE ON chmel TO xbabic09;
GRANT INSERT, UPDATE ON kvasnice TO xbabic09;
GRANT INSERT, SELECT ON recept TO xbabic09;
GRANT UPDATE ON uzivatel TO xbabic09;
GRANT ALL ON sladek TO xbabic09;

GRANT EXECUTE ON podvodnici TO xbabic09;
GRANT EXECUTE ON uroven_predajni TO xbabic09;

--materialized view clena xbabic09
DROP MATERIALIZED VIEW hodnotene_piva;

CREATE MATERIALIZED VIEW hodnotene_piva
BUILD IMMEDIATE
REFRESH ON COMMIT AS
    SELECT XPESKO00.sladek.SladekID AS ID_SLADEK, XPESKO00.pivo.Nazov AS NAZOV_PIVO
    FROM XPESKO00.sladek JOIN XPESKO00.pivo ON XPESKO00.sladek.SladekID = XPESKO00.pivo.varil_sladek
ORDER BY XPESKO00.pivo.nazov;

SELECT * FROM hodnotene_piva;

INSERT INTO XPESKO00.pivo(PivoID, Nazov, Farba_EBC, Styl_kvasenia, Typ, Obsah_alkoholu, Obdobie_varenia, varil_sladek, varil_pivovar)
VALUES ('004', 'SpongeBob Squarepants', '33', 'spontanni', 'lezak', '0.6', 'vcera', '002', '003');

SELECT * FROM hodnotene_piva;

COMMIT;

SELECT * FROM hodnotene_piva;
