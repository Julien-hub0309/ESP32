  let esp32Url = ""; // URL de ton ESP32 (ex: http://192.168.1.42)

    async function connectESP32() {
      // Ici tu définis l’adresse IP ou le hostname de ton ESP32
      esp32Url = prompt("Entrez l'adresse IP de l'ESP32 (ex: http://192.168.1.42)");
      if (esp32Url) {
        alert("Connecté à l’ESP32 via WiFi !");
      } else {
        alert("Adresse non valide !");
      }
    }

    async function setLed(state) {
      if (!esp32Url) {
        alert("Pas connecté !");
        return;
      }
      try {
        const response = await fetch(`${esp32Url}/led?state=${state}`);
        const data = await response.text();
        document.getElementById("status").innerText = "Statut : " + data;
      } catch (error) {
        console.error(error);
        alert("Erreur de communication : " + error);
      }
    }

    document.getElementById("connect").addEventListener("click", connectESP32);
    document.getElementById("on").addEventListener("click", () => setLed("ON"));
    document.getElementById("off").addEventListener("click", () => setLed("OFF"));