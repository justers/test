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
-- База данных: `main_server`
--

-- --------------------------------------------------------

--
-- Структура таблицы `characters`
--

CREATE TABLE IF NOT EXISTS `characters` (
  `c_id` int(11) NOT NULL AUTO_INCREMENT,
  `c_profile` int(11) NOT NULL,
  `c_login` varchar(32) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `c_level` int(4) NOT NULL DEFAULT '1',
  `c_exp` int(6) NOT NULL DEFAULT '0',
  `c_age` int(4) NOT NULL,
  `c_gender` int(2) NOT NULL,
  `c_skin` int(6) NOT NULL,
  `c_money` int(11) NOT NULL DEFAULT '20',
  PRIMARY KEY (`c_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=cp1251 AUTO_INCREMENT=2 ;

--
-- Дамп данных таблицы `characters`
--

INSERT INTO `characters` (`c_id`, `c_profile`, `c_login`, `c_level`, `c_exp`, `c_age`, `c_gender`, `c_skin`, `c_money`) VALUES
(1, 1, 'Leo_Juster', 1, 0, 18, 1, 79, 20);

-- --------------------------------------------------------

--
-- Структура таблицы `ownable_veh_system`
--

CREATE TABLE IF NOT EXISTS `ownable_veh_system` (
  `v_id` int(11) NOT NULL AUTO_INCREMENT,
  `v_model` int(4) NOT NULL,
  `v_owner` int(11) NOT NULL,
  `v_pos_x` float NOT NULL,
  `v_pos_y` float NOT NULL,
  `v_pos_z` float NOT NULL,
  `v_pos_c` float NOT NULL,
  `v_color_one` int(4) NOT NULL,
  `v_color_two` int(4) NOT NULL,
  `v_millage` float NOT NULL,
  `v_fuel` float NOT NULL,
  `v_plate` varchar(32) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`v_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Структура таблицы `server_settings`
--

CREATE TABLE IF NOT EXISTS `server_settings` (
  `can_register` int(3) NOT NULL DEFAULT '1',
  `can_enter` int(3) NOT NULL DEFAULT '1'
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;

--
-- Дамп данных таблицы `server_settings`
--

INSERT INTO `server_settings` (`can_register`, `can_enter`) VALUES
(1, 1);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
