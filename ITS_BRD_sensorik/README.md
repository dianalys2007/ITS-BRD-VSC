# ITS_BRD_sensorik

Dieses Repository ist ein Submodul für das ITS-BRD-VSC-Repository (bzw. Ihren Fork davon). Es enthält die Treiber und Projektvorlagen für

- **ADC**: Analog-Digital-Wandler des Nucleo-Boards
- **IKS01A3**: Sensor-Shield X-NUCLEO-IKS01A3 (Temperatur, Luftfeuchtigkeit, Druck, Beschleunigung, Magnetfeld, Gyroskop)

# Submodul einbinden

Das Repository ist privat. Sie benötigen Lesezugriff auf GitHub in der Organisation ITS-BRD. Wie sie diesen Zugriff bekommen ist z.B. in "2_VSC_GitProjekt_aus_GitHub_holen.pdf" beschrieben (siehe Moodle/Teams).
Sie müssen bei Git angemeldet sein, z. B. mit `gh auth login` oder über die Browser-Anmeldung des Git Credential Managers. Wenn Git nach einem Passwort fragt, geben Sie stattdessen ein [Personal Access Token](https://docs.github.com/de/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens) mit Leserechten auf das Repository ein.

1. Öffnen Sie das Terminal in VS Code durch Klicken auf "Terminal" > "Neues Terminal" in der Menüleiste oder durch Drücken von `Strg+Shift+ö` (Windows/Linux) oder `Cmd+Shift+ö` (macOS).
2. Stellen Sie sicher, dass Sie sich im Hauptverzeichnis Ihres Forks (`ITS-BRD-VSC`) befinden.
3. Führen Sie den folgenden Befehl aus, um das Submodul hinzuzufügen:

``` git submodule add https://github.com/ITS-BRD/ITS_BRD_sensorik.git ```

4. Übernehmen Sie die Änderung in Ihren Fork:

``` git commit -m "Submodul ITS_BRD_sensorik hinzugefügt" ```

``` git push ```

Wenn Sie Ihren Fork später neu klonen, laden Sie das Submodul mit dem Befehl `git submodule update --init --recursive` herunter.

# Neues Projekt erstellen

1. Öffnen Sie das Terminal in VS Code und gehen Sie mit Hilfe von cd (Change Directory) in den Ordner `ITS_BRD_sensorik` (ITS-BRD-VSC/ITS_BRD_sensorik).
2. Führen Sie den Befehl für die gewünschte Projektart aus:

| Projektart | Windows | macOS / Linux |
|---|---|---|
| ADC | `createNewProjectADC.bat` | `./createNewProjectADC.sh` |
| IKS01A3 | `createNewProjectIKS01A3.bat` | `./createNewProjectIKS01A3.sh` |

3. Sie werden aufgefordert, einen Projektnamen einzugeben. Der Name muss mit einem Großbuchstaben beginnen und darf nur Buchstaben und Ziffern enthalten.

Das neue Projekt wird im Ordner `ITS-BRD-VSC/Programs/<Name des Projekts>` angelegt. Wie Sie es in VS Code auswählen, builden und auf das Board übertragen, steht in der README des Hauptrepositories (Abschnitte "Neues Projekt erstellen" und "Building").
