--
-- Archivo generado con SQLiteStudio v3.4.4 el mi. oct. 18 18:52:44 2023
--
-- Codificación de texto usada: System
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Tabla: Settings
CREATE TABLE IF NOT EXISTS Settings (screen_window_width INTEGER DEFAULT (900), screen_window_height INTEGER DEFAULT (900), fps_fps TEXT DEFAULT (60));

COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
