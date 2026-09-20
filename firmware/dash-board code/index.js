import { initializeApp }
        from "https://www.gstatic.com/firebasejs/12.0.0/firebase-app.js";
    import {
        getDatabase,
        ref,
        onValue
    } from "https://www.gstatic.com/firebasejs/12.0.0/firebase-database.js";
    const firebaseConfig = {
        apiKey: "AIzaSyAVLyKW2jF429BHGJw9Kc7nmq5iD9BbrMQ",
        authDomain: "optigrow-a8ee2.firebaseapp.com",
        projectId: "optigrow-a8ee2",
        storageBucket: "optigrow-a8ee2.firebasestorage.app",
        messagingSenderId: "420109885084",
        appId: "1:420109885084:web:fc9523938e8d737dae5759",
        measurementId: "G-B8SCC6PNMD"
    };
    const app = initializeApp(firebaseConfig);
    const db = getDatabase(app);
    const sensorsRef = ref(db, "sensors");
    onValue(sensorsRef, (snapshot) => {
        const data = snapshot.val();
        console.log("Sensor data:", data);
        if (!data) return;
        document.querySelector("#temperature .value").textContent =
            data.temperature ?? 0;
        document.querySelector("#humidity .value").textContent =
            data.humidity ?? 0;
        document.querySelector("#soil .value").textContent =
            data.soil ?? 0;
        document.querySelector("#light .value").textContent =
            data.light ?? 0;
    });