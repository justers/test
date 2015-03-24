-- phpMyAdmin SQL Dump
-- version 3.5.1
-- http://www.phpmyadmin.net
--
-- Хост: 127.0.0.1
-- Время создания: Мар 07 2015 г., 20:53
-- Версия сервера: 5.5.25
-- Версия PHP: 5.3.13

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `main_cross_server`
--

-- --------------------------------------------------------

--
-- Структура таблицы `cross_server_chat`
--

CREATE TABLE IF NOT EXISTS `cross_server_chat` (
  `to_p_increment` int(11) NOT NULL,
  `from_p_increment` int(11) NOT NULL,
  `message_text` varchar(144) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;

-- --------------------------------------------------------

--
-- Структура таблицы `profiles`
--

CREATE TABLE IF NOT EXISTS `profiles` (
  `p_login` varchar(32) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `p_increment` int(11) NOT NULL AUTO_INCREMENT,
  `p_password` varchar(16) CHARACTER SET cp1250 COLLATE cp1250_bin NOT NULL,
  `p_ip_registered` varchar(18) CHARACTER SET cp1250 COLLATE cp1250_bin NOT NULL,
  `p_ip_last` varchar(18) CHARACTER SET cp1250 COLLATE cp1250_bin NOT NULL,
  `p_date_registered` varchar(32) CHARACTER SET cp1251 COLLATE cp1251_bin NOT NULL,
  `p_date_last` varchar(32) CHARACTER SET cp1250 COLLATE cp1250_bin NOT NULL,
  `p_is_online` int(3) NOT NULL,
  `p_email` varchar(32) CHARACTER SET cp1250 COLLATE cp1250_bin NOT NULL,
  PRIMARY KEY (`p_increment`)
) ENGINE=MyISAM  DEFAULT CHARSET=cp1251 AUTO_INCREMENT=2 ;

--
-- Дамп данных таблицы `profiles`
--

INSERT INTO `profiles` (`p_login`, `p_increment`, `p_password`, `p_ip_registered`, `p_ip_last`, `p_date_registered`, `p_date_last`, `p_is_online`, `p_email`) VALUES
('Juster', 1, 'nokia13', '127.0.0.1', '255.255.255.255', '2015-03-07 17:06:22', '2015-03-07 19:50:48', 0, '');

-- --------------------------------------------------------

--
-- Структура таблицы `teleport_areas`
--

CREATE TABLE IF NOT EXISTS `teleport_areas` (
  `area_id` int(11) NOT NULL AUTO_INCREMENT,
  `area_x` float NOT NULL,
  `area_y` float NOT NULL,
  `area_z` float NOT NULL,
  `area_c` float NOT NULL,
  `area_virtual_world` int(11) NOT NULL,
  `area_interior` int(11) NOT NULL,
  `area_to_x` float NOT NULL,
  `area_to_y` float NOT NULL,
  `area_to_z` float NOT NULL,
  `area_to_c` float NOT NULL,
  `area_to_int` int(11) NOT NULL,
  `area_to_virtual_world` int(11) NOT NULL,
  `area_info` varchar(144) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`area_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
