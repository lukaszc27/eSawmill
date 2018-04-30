-- MySQL dump 10.13  Distrib 5.7.17, for Win64 (x86_64)
--
-- Host: localhost    Database: sawmill
-- ------------------------------------------------------
-- Server version	5.7.20-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `adres`
--

DROP TABLE IF EXISTS `adres`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `adres` (
  `adresId` int(11) NOT NULL AUTO_INCREMENT,
  `kontrahentId` int(11) NOT NULL,
  `kod_pocztowy` varchar(64) DEFAULT NULL,
  `poczta` varchar(64) DEFAULT NULL,
  `miejscowosc` varchar(64) DEFAULT NULL,
  `ulica` varchar(64) DEFAULT NULL,
  `nr_lokalu` varchar(16) DEFAULT NULL,
  `telefon` varchar(64) DEFAULT NULL,
  `email` varchar(64) DEFAULT NULL,
  `www` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`adresId`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `adres`
--

LOCK TABLES `adres` WRITE;
/*!40000 ALTER TABLE `adres` DISABLE KEYS */;
INSERT INTO `adres` VALUES (7,7,'38-305','LIPINKI','LIPINKI','','','','',''),(8,8,'38-305','LIPINKI','WÓJTOWA','','','781 791 712','',''),(9,9,'38-305','LIPINKI','LIPA','','','791 610 125','',''),(11,11,'38-340','','STRZESZYN','','','668 255 708','',''),(15,15,'','','','','','','',''),(16,16,'38-305','LIPINKI','LIPINKI','','626','661 233 757','LUKASZCIESLA52@GMAIL.COM','WWW.FACEBOOK.COM/LUKASZC27'),(17,17,'38-305','LIPINKI','LIPINKI','','556','321 654 987','MICHAL.PIROG@GMAIL.COM','');
/*!40000 ALTER TABLE `adres` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `artykuly`
--

DROP TABLE IF EXISTS `artykuly`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `artykuly` (
  `idArtykulu` int(11) NOT NULL AUTO_INCREMENT,
  `idMagazynu` int(11) NOT NULL,
  `nazwa` varchar(128) NOT NULL,
  `cenaNetto` float NOT NULL,
  `cenaBrutto` float DEFAULT NULL,
  `vat` int(11) DEFAULT NULL,
  `jm` varchar(32) DEFAULT NULL,
  `cenaDetaliczna` float NOT NULL,
  `marza` float DEFAULT NULL,
  PRIMARY KEY (`idArtykulu`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `artykuly`
--

LOCK TABLES `artykuly` WRITE;
/*!40000 ALTER TABLE `artykuly` DISABLE KEYS */;
INSERT INTO `artykuly` VALUES (1,1,'ZRZYNY NIEOBRZYNANE',1,1.23,23,'mb',1.23,0),(2,1,'ZRZYNY OBRZYNANE',1.1,1.35,23,'mb',1.35,0);
/*!40000 ALTER TABLE `artykuly` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `artykuly_magazyn`
--

DROP TABLE IF EXISTS `artykuly_magazyn`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `artykuly_magazyn` (
  `idMagazynu` int(11) NOT NULL AUTO_INCREMENT,
  `nazwa` varchar(128) NOT NULL,
  `opis` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`idMagazynu`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `artykuly_magazyn`
--

LOCK TABLES `artykuly_magazyn` WRITE;
/*!40000 ALTER TABLE `artykuly_magazyn` DISABLE KEYS */;
INSERT INTO `artykuly_magazyn` VALUES (1,'MAGAZYN GŁÓWNY (MG)','GŁÓWNY MAGAZYN ESAWMILL'),(4,'BUAZERIA','PRZECHOWUJE BUAZERIĘ ORAZ ELEMENTY WYKOŃCZENIOWE');
/*!40000 ALTER TABLE `artykuly_magazyn` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `elementyuslugi`
--

DROP TABLE IF EXISTS `elementyuslugi`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `elementyuslugi` (
  `idUslugi` int(11) NOT NULL,
  `szerokosc` float DEFAULT NULL,
  `wysokosc` float DEFAULT NULL,
  `dlugosc` float DEFAULT NULL,
  `ilosc` float DEFAULT NULL,
  `strugane` tinyint(1) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Przechowuje elementy do wycięcia z drewna dostarczonego przez klienta';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `elementyuslugi`
--

LOCK TABLES `elementyuslugi` WRITE;
/*!40000 ALTER TABLE `elementyuslugi` DISABLE KEYS */;
/*!40000 ALTER TABLE `elementyuslugi` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `elementyzamowienia`
--

DROP TABLE IF EXISTS `elementyzamowienia`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `elementyzamowienia` (
  `elementId` int(11) NOT NULL AUTO_INCREMENT,
  `zamowienieId` int(11) NOT NULL,
  `szerokosc` float DEFAULT NULL,
  `wysokosc` float DEFAULT NULL,
  `dlugosc` float DEFAULT NULL,
  `ilosc` float DEFAULT NULL,
  `strugane` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`elementId`)
) ENGINE=InnoDB AUTO_INCREMENT=262 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `elementyzamowienia`
--

LOCK TABLES `elementyzamowienia` WRITE;
/*!40000 ALTER TABLE `elementyzamowienia` DISABLE KEYS */;
INSERT INTO `elementyzamowienia` VALUES (154,7,8,18,7.5,32,0),(155,7,14,14,12,4,0),(156,7,18,24,12,1,0),(157,7,8,18,5.22,14,0),(158,7,8,18,1.6,16,0),(159,7,16,16,2,2,0),(160,7,14,14,1,4,0),(161,5,16,10,6.5,2,0),(162,5,16,10,6,2,0),(163,5,16,4,4.5,30,0),(164,5,16,4,3.5,3,0),(165,5,12,12,2.3,6,1),(166,5,12,12,2.8,1,1),(167,5,10,6,2.5,6,0),(208,10,14,14,5.5,4,0),(209,10,14,14,5,4,0),(210,10,18,18,4,2,0),(211,10,18,18,3,2,0),(212,10,18,14,3,6,0),(213,10,8,18,7.2,8,0),(214,10,8,18,6,8,0),(215,10,8,18,5,8,0),(216,10,8,18,4.2,8,0),(217,10,8,18,3.2,8,0),(218,10,8,18,2.7,8,0),(219,10,8,18,1.8,8,0),(220,10,3.2,18,60,1,0),(221,10,4,5,570,1,0),(222,10,2.5,6,280,1,0),(223,6,16,12,7.5,4,0),(224,6,14.5,14.5,3.6,4,1),(225,6,14.5,14.5,2.5,6,1),(242,11,14,14,5.5,4,0),(243,11,14,14,5,4,0),(244,11,18,18,4,2,0),(245,11,18,18,3,2,0),(246,11,18,14,3,6,0),(247,11,8,18,7.2,8,0),(248,11,8,18,6,8,0),(249,11,8,18,5,8,0),(250,11,8,18,4.2,8,0),(251,11,8,18,3.2,8,0),(252,11,8,18,2.7,8,0),(253,11,8,18,1.8,8,0),(254,11,3.2,18,60,1,0),(255,11,4,5,570,1,0),(256,11,2.5,6,280,1,0),(257,12,12,6,1.2,6,0),(258,12,12,6,2.2,24,0),(259,12,12,6,2.3,36,0),(260,12,12,12,8,2,0),(261,12,12,12,4,2,0);
/*!40000 ALTER TABLE `elementyzamowienia` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `klodyuslugi`
--

DROP TABLE IF EXISTS `klodyuslugi`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `klodyuslugi` (
  `idUslugi` int(11) NOT NULL,
  `srednica` float NOT NULL,
  `dlugosc` float NOT NULL,
  `kantowana` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Tabela przechowująca kłody przeznaczone do usług';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `klodyuslugi`
--

LOCK TABLES `klodyuslugi` WRITE;
/*!40000 ALTER TABLE `klodyuslugi` DISABLE KEYS */;
INSERT INTO `klodyuslugi` VALUES (1,40,8,1);
/*!40000 ALTER TABLE `klodyuslugi` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary view structure for view `kontrahenci`
--

DROP TABLE IF EXISTS `kontrahenci`;
/*!50001 DROP VIEW IF EXISTS `kontrahenci`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `kontrahenci` AS SELECT 
 1 AS `id`,
 1 AS `firma`,
 1 AS `imie`,
 1 AS `nazwisko`,
 1 AS `nip`,
 1 AS `regon`,
 1 AS `pesel`,
 1 AS `kod_pocztowy`,
 1 AS `poczta`,
 1 AS `miejscowosc`,
 1 AS `ulica`,
 1 AS `nr_lokalu`,
 1 AS `telefon`,
 1 AS `email`,
 1 AS `www`,
 1 AS `dluznik`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `kontrahent`
--

DROP TABLE IF EXISTS `kontrahent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kontrahent` (
  `kontrahentId` int(11) NOT NULL AUTO_INCREMENT,
  `firma` varchar(128) DEFAULT NULL,
  `imie` varchar(64) DEFAULT NULL,
  `nazwisko` varchar(64) DEFAULT NULL,
  `nip` varchar(64) DEFAULT NULL,
  `regon` varchar(64) DEFAULT NULL,
  `pesel` varchar(64) DEFAULT NULL,
  `dluznik` tinyint(4) DEFAULT '0',
  PRIMARY KEY (`kontrahentId`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kontrahent`
--

LOCK TABLES `kontrahent` WRITE;
/*!40000 ALTER TABLE `kontrahent` DISABLE KEYS */;
INSERT INTO `kontrahent` VALUES (7,'','ŁUKASZ','PIRÓG','','','',0),(8,'','','WROSZCZAK','','','',0),(9,'','MARIAN','ŚWIERZ','','','',0),(11,'','DARIUSZ','SIWULSKI','','','',0),(15,'','','BRONGIEL','','','',0),(16,'','LUKASZ','CIESLA','','','96080610259',0),(17,'','MICHAL','PIROG','','','',0);
/*!40000 ALTER TABLE `kontrahent` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `uslugi`
--

DROP TABLE IF EXISTS `uslugi`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `uslugi` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tytul` varchar(128) NOT NULL,
  `dataDodania` date NOT NULL,
  `termin` date NOT NULL,
  `zrealizowane` tinyint(1) NOT NULL,
  `rabat` float DEFAULT NULL,
  `idKontrahenta` int(11) NOT NULL,
  `uwagi` varchar(512) DEFAULT NULL,
  `priorytet` int(11) DEFAULT '1',
  `vat` float DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Tabela przechowująca informacje o usługach';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `uslugi`
--

LOCK TABLES `uslugi` WRITE;
/*!40000 ALTER TABLE `uslugi` DISABLE KEYS */;
INSERT INTO `uslugi` VALUES (1,'','2018-02-03','2018-03-01',0,0,9,'',1,0);
/*!40000 ALTER TABLE `uslugi` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `uslugi_artykuly`
--

DROP TABLE IF EXISTS `uslugi_artykuly`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `uslugi_artykuly` (
  `idUslugi` int(11) NOT NULL,
  `idArtykulu` int(11) NOT NULL,
  `ilosc` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `uslugi_artykuly`
--

LOCK TABLES `uslugi_artykuly` WRITE;
/*!40000 ALTER TABLE `uslugi_artykuly` DISABLE KEYS */;
INSERT INTO `uslugi_artykuly` VALUES (1,1,10);
/*!40000 ALTER TABLE `uslugi_artykuly` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary view structure for view `uslugiwidok`
--

DROP TABLE IF EXISTS `uslugiwidok`;
/*!50001 DROP VIEW IF EXISTS `uslugiwidok`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `uslugiwidok` AS SELECT 
 1 AS `tytul`,
 1 AS `firma`,
 1 AS `imie`,
 1 AS `nazwisko`,
 1 AS `dataDodania`,
 1 AS `termin`,
 1 AS `uwagi`,
 1 AS `zrealizowane`,
 1 AS `id`,
 1 AS `idKontrahenta`,
 1 AS `rabat`,
 1 AS `priorytet`,
 1 AS `vat`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `zamowienia`
--

DROP TABLE IF EXISTS `zamowienia`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zamowienia` (
  `zamowienieId` int(11) NOT NULL AUTO_INCREMENT,
  `kontrahentId` int(11) NOT NULL,
  `dataPrzyjecia` date DEFAULT NULL,
  `terminRealizacji` date DEFAULT NULL,
  `opis` varchar(128) DEFAULT NULL,
  `zrealizowane` tinyint(1) DEFAULT NULL,
  `naddatek` float DEFAULT NULL,
  `rabat` float DEFAULT NULL,
  `uwagi` varchar(512) DEFAULT NULL,
  `priorytet` int(11) DEFAULT '1',
  `vat` int(11) NOT NULL DEFAULT '0',
  `cena` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`zamowienieId`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `zamowienia`
--

LOCK TABLES `zamowienia` WRITE;
/*!40000 ALTER TABLE `zamowienia` DISABLE KEYS */;
INSERT INTO `zamowienia` VALUES (5,7,'2018-01-13','2018-01-01','DACH',1,0,0,'',1,0,750),(6,7,'2018-01-13','2018-01-04','DOMEK DREWNIANY',1,0,0,'',1,0,750),(7,8,'2018-01-13','2018-01-19','DACH',0,0,0,'',1,0,750),(10,15,'2018-01-25','2018-01-28','',0,0,0,'',0,0,750),(11,16,'2018-02-11','2018-01-31','DACH',0,0,0,'',0,0,750),(12,17,'2018-04-26','2018-05-24','',0,0,0,'Dostawa po godzinie 16',1,0,750);
/*!40000 ALTER TABLE `zamowienia` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary view structure for view `zamowieniawidok`
--

DROP TABLE IF EXISTS `zamowieniawidok`;
/*!50001 DROP VIEW IF EXISTS `zamowieniawidok`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `zamowieniawidok` AS SELECT 
 1 AS `opis`,
 1 AS `firma`,
 1 AS `imie`,
 1 AS `nazwisko`,
 1 AS `dataPrzyjecia`,
 1 AS `terminRealizacji`,
 1 AS `zamowienieId`,
 1 AS `zrealizowane`,
 1 AS `rabat`,
 1 AS `priorytet`,
 1 AS `vat`,
 1 AS `cena`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `zamowienie_artykuly`
--

DROP TABLE IF EXISTS `zamowienie_artykuly`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zamowienie_artykuly` (
  `idZamowienia` int(11) NOT NULL,
  `idArtykulu` int(11) NOT NULL,
  `ilosc` float NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Przechowywuje zapisane artyukły w zamówieniu';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `zamowienie_artykuly`
--

LOCK TABLES `zamowienie_artykuly` WRITE;
/*!40000 ALTER TABLE `zamowienie_artykuly` DISABLE KEYS */;
INSERT INTO `zamowienie_artykuly` VALUES (6,1,10),(11,1,45),(11,2,120),(12,1,5);
/*!40000 ALTER TABLE `zamowienie_artykuly` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'sawmill'
--
/*!50003 DROP FUNCTION IF EXISTS `artykuly_dodajMagazyn` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `artykuly_dodajMagazyn`(
	m_nazwa varchar(128),
    m_opis varchar(512)
    ) RETURNS int(11)
begin
	insert into artykuly_magazyn (nazwa, opis) values(m_nazwa, m_opis);
    
    set @magazynId = 0;
    select max(idMagazynu) into @magazynId from artykuly_magazyn;
    
    return @magazynId;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `DodajKontrahenta` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `DodajKontrahenta`(
	m_firma			varchar(128),
	m_imie 			varchar(64),
    m_nazwisko 		varchar(64),
    m_nip			varchar(64),
    m_regon 		varchar(64),
    m_pesel 		varchar(64),
    m_kod_pocztowy	varchar(64),
    m_poczta 		varchar(64),
    m_miejscowosc	varchar(64),
    m_ulica			varchar(64),
    m_nr_lokalu		varchar(32),
    m_telefon		varchar(64),
    m_email 		varchar(64),
    m_www 			varchar(64)
) RETURNS int(11)
begin
	insert into Kontrahent (firma, imie, nazwisko, nip, regon, pesel)
				values(m_firma, m_imie, m_nazwisko, m_nip, m_regon, m_pesel);
	
    set @id = 0;
    select max(kontrahentId) into @id from Kontrahent;
    
    insert into Adres (kontrahentId, kod_pocztowy, poczta, miejscowosc, ulica, nr_lokalu,
						telefon, email, www)
				values(@id, m_kod_pocztowy, m_poczta, m_miejscowosc, m_ulica, m_nr_lokalu,
						m_telefon, m_email, m_www);
	
    return @id;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `dodajUsluge` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `dodajUsluge`(
	m_id int,
    m_idKontrahenta int,
    m_tytul varchar(128),
    m_dataDodania date,
    m_termin date,
    m_zrealizowane boolean,
    m_rabat float,
    m_priorytet int,
    m_vat float,
    m_notatki varchar(512)
) RETURNS int(11)
begin
	declare id int;

    if m_id > 0 then
		update uslugi set tytul = m_tytul, dataDodania = m_dataDodania, 
							termin = m_termin, zrealizowane = m_zrealizowane, 
                            rabat = m_rabat, idKontrahenta = m_idKontrahenta, uwagi = m_notatki,
                            priorytet = m_priorytet, vat = m_vat
					  where uslugi.id = m_id;
		return m_id;
	else
		insert into uslugi (tytul, dataDodania, termin, zrealizowane, rabat, idKontrahenta, uwagi, priorytet, vat)
				values(m_tytul, m_dataDodania, m_termin, m_zrealizowane, m_rabat, m_idKontrahenta, m_notatki, 
					m_priorytet, m_vat);
			
        set @id = 0;
        select max(uslugi.id) into @id from uslugi;
        
        return @id;
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `DodajZamowienie` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `DodajZamowienie`(
	m_zamowienieId		int,
	m_kontrahentId 		int,
    m_dataPrzyjecia		date,
    m_terminRealizacji 	date,
    m_opis 				varchar(128),
    m_zrealizowane 		boolean,
    m_naddatek 			float,
    m_rabat				float,
    m_uwagi				varchar(512),
    m_priorytet			int,
    m_vat				int,
    m_cena				float
) RETURNS int(11)
begin
	declare id int;
    
    if m_zamowienieId > 0 then
		update zamowienia set kontrahentId = m_kontrahentId, dataPrzyjecia = m_dataPrzyjecia,
					terminRealizacji = m_terminRealizacji, opis = m_opis,
                    zrealizowane = m_zrealizowane, naddatek = m_naddatek,
                    rabat = m_rabat, uwagi = m_uwagi, priorytet = m_priorytet, 
                    vat = m_vat, cena = m_cena where zamowienieId = m_zamowienieId;
		
        /* czyścimy listę elementów przy aktualizacji */
		delete from elementyZamowienia where zamowienieId = m_zamowienieId;
        delete from zamowienie_artykuly where idZamowienia = m_zamowienieId;
        
        return m_zamowienieId;
	else
		insert into zamowienia (kontrahentId, dataPrzyjecia, terminRealizacji, opis, 
						zrealizowane, naddatek, rabat, uwagi, priorytet, vat, cena)
		values(m_kontrahentId, m_dataPrzyjecia, m_terminRealizacji, m_opis, m_zrealizowane, 
        m_naddatek, m_rabat, m_uwagi, m_priorytet, m_vat, m_cena);
        
        set @id = 0;
        select max(zamowienieId) into @id from zamowienia;
        return @id;
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `artykuly_dodaj` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `artykuly_dodaj`(
	in m_idMagazynu int,
    in m_nazwa varchar(128),
    in m_cenaNetto float,
    in m_vat int,
    in m_cenaBrutto float,
    in m_jm varchar(32),
    in m_cenaDetaliczna float,
    in m_marza float,
    in m_artykulId int
)
begin
	if m_artykulId > 0 then
		update artykuly set idMagazynu = m_idMagazynu, nazwa = m_nazwa, vat = m_vat, cenaNetto = m_cenaNetto,
			cenaBrutto = m_cenaBrutto, jm = m_jm, marza = m_marza,
            cenaDetaliczna = m_cenaDetaliczna where idArtykulu = m_artykulId;
	else
		insert into artykuly (idMagazynu, nazwa, cenaNetto, cenaBrutto, vat, jm, cenaDetaliczna, marza)
				values(m_idMagazynu, m_nazwa, m_cenaNetto, m_cenaBrutto, m_vat, m_jm, m_cenaDetaliczna, m_marza);
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `artykuly_usun` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `artykuly_usun`(IN m_id INT)
BEGIN
	DELETE FROM artykuly WHERE idArtykulu = m_id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DodajElementZamowienia` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DodajElementZamowienia`(
	in m_zamowienieId 	int,
    in m_szerokosc		float,
    in m_wysokosc		float,
    in m_dlugosc		float,
    in m_ilosc			float,
    in m_strugane		boolean
)
begin
	insert into ElementyZamowienia (zamowienieId, szerokosc, wysokosc, dlugosc, ilosc, strugane)
		values(m_zamowienieId, m_szerokosc, m_wysokosc, m_dlugosc, m_ilosc, m_strugane);
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `dodajKlode` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `dodajKlode`(in m_idUslugi int,
							in m_srednica float,
                            in m_dlugosc float,
                            in m_kantowana boolean)
begin
	insert into klodyUslugi (idUslugi, srednica, dlugosc, kantowana)
			values(m_idUslugi, m_srednica, m_dlugosc, m_kantowana);
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `Kontrahent_OznaczDluznika` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `Kontrahent_OznaczDluznika`(in id int)
begin
	update kontrahent set dluznik = 1 where kontrahentId = id;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `UsunElementZamowienia` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `UsunElementZamowienia`(
	in m_id int
)
begin
	delete from ElementyZamowienia where elementId = m_id;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `usunKontrahenta` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `usunKontrahenta`(in m_id int)
begin
	delete from Kontrahent where kontrahentId = m_id;
    delete from Adres where kontrahentid = m_id;
    
    set @idZamowienia = 0;
    select zamowienieId into @idZamowienia from zamowienia where kontrahentId = m_Id;
    
    if @idZamowienia > 0 then
		delete from zamowienia where kontrahentId = m_id;
		delete from elementyzamowienia where zamowienieId = @idZamowienia;
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `usunUsluge` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `usunUsluge`(in id int)
begin
	delete from uslugi where uslugi.id = id;
    delete from klodyuslugi where klodyuslugi.idUslugi = id;
    delete from elementyuslugi where elementyuslugi.idUslugi = id;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `UsunZamowienie` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `UsunZamowienie`(
	in m_id int
)
begin
	delete from Zamowienia where zamowienieId = m_id;
    delete from ElementyZamowienia where zamowienieId = m_id;
    delete from zamowienie_artykuly where idZamowienia = m_id;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `wyczyscElementyZamowienia` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `wyczyscElementyZamowienia`(in id int)
begin
	set @x = 0;
    select count(zamowienieId) into @x from elementyzamowienia where zamowienieId = id;
    
    if @x > 0 then
		delete from elementyzamowienia where zamowienieId = id;
    end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `wyczyscUsluge` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `wyczyscUsluge`(in id int)
begin
	set @x = 0;
    select count(klodyuslugi.idUslugi) into @x from klodyuslugi;
    
    if @x > 0 then
		delete from klodyuslugi where idUslugi = id;
	end if;
    
    set @x = 0;
    select count(elementyuslugi.idUslugi) into @x from elementyuslugi;
    if @x > 0 then
		delete from elementyuslugi where idUslugi = id;
	end if;
    
    set @x = 0;
    select count(uslugi_artykuly.idUslugi) into @x from uslugi_artykuly;
    if @x > 0 then
		delete from uslugi_artykuly where idUslugi = id;
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `Zamowienie_Zrealizuj` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `Zamowienie_Zrealizuj`(in id int)
begin
	update zamowienia set zrealizowane=1 where zamowienieId = id;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Final view structure for view `kontrahenci`
--

/*!50001 DROP VIEW IF EXISTS `kontrahenci`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `kontrahenci` AS select `kontrahent`.`kontrahentId` AS `id`,`kontrahent`.`firma` AS `firma`,`kontrahent`.`imie` AS `imie`,`kontrahent`.`nazwisko` AS `nazwisko`,`kontrahent`.`nip` AS `nip`,`kontrahent`.`regon` AS `regon`,`kontrahent`.`pesel` AS `pesel`,`adres`.`kod_pocztowy` AS `kod_pocztowy`,`adres`.`poczta` AS `poczta`,`adres`.`miejscowosc` AS `miejscowosc`,`adres`.`ulica` AS `ulica`,`adres`.`nr_lokalu` AS `nr_lokalu`,`adres`.`telefon` AS `telefon`,`adres`.`email` AS `email`,`adres`.`www` AS `www`,`kontrahent`.`dluznik` AS `dluznik` from (`kontrahent` join `adres`) where (`kontrahent`.`kontrahentId` = `adres`.`kontrahentId`) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `uslugiwidok`
--

/*!50001 DROP VIEW IF EXISTS `uslugiwidok`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `uslugiwidok` AS select `uslugi`.`tytul` AS `tytul`,`kontrahent`.`firma` AS `firma`,`kontrahent`.`imie` AS `imie`,`kontrahent`.`nazwisko` AS `nazwisko`,`uslugi`.`dataDodania` AS `dataDodania`,`uslugi`.`termin` AS `termin`,`uslugi`.`uwagi` AS `uwagi`,`uslugi`.`zrealizowane` AS `zrealizowane`,`uslugi`.`id` AS `id`,`uslugi`.`idKontrahenta` AS `idKontrahenta`,`uslugi`.`rabat` AS `rabat`,`uslugi`.`priorytet` AS `priorytet`,`uslugi`.`vat` AS `vat` from (`uslugi` join `kontrahent`) where (`uslugi`.`idKontrahenta` = `kontrahent`.`kontrahentId`) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `zamowieniawidok`
--

/*!50001 DROP VIEW IF EXISTS `zamowieniawidok`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `zamowieniawidok` AS select `zamowienia`.`opis` AS `opis`,`kontrahent`.`firma` AS `firma`,`kontrahent`.`imie` AS `imie`,`kontrahent`.`nazwisko` AS `nazwisko`,`zamowienia`.`dataPrzyjecia` AS `dataPrzyjecia`,`zamowienia`.`terminRealizacji` AS `terminRealizacji`,`zamowienia`.`zamowienieId` AS `zamowienieId`,`zamowienia`.`zrealizowane` AS `zrealizowane`,`zamowienia`.`rabat` AS `rabat`,`zamowienia`.`priorytet` AS `priorytet`,`zamowienia`.`vat` AS `vat`,`zamowienia`.`cena` AS `cena` from (`zamowienia` join `kontrahent`) where (`zamowienia`.`kontrahentId` = `kontrahent`.`kontrahentId`) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-28 17:43:14
