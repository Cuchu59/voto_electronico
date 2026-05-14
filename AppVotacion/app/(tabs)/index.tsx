import { CameraView, useCameraPermissions } from 'expo-camera'; // Importamos la cámara
import React, { useState } from 'react';
import { Alert, Button, ScrollView, StyleSheet, Text, TouchableOpacity, View } from 'react-native';

const CANDIDATOS = [
  { id: 1, nombre: 'Cuchu', partido: 'La Cámpora' },
  { id: 2, nombre: 'Javier Miguel', partido: 'La Libertad Avanza' },
  { id: 3, nombre: 'LA REINA', partido: 'Critina libree' },
  { id: 4, nombre: 'Javier Toobe', partido: 'ESP Lovers' },
  { id: 5, nombre: 'Momo', partido: 'Platense' },
  { id: 6, nombre: 'Voto en Blanco', partido: '-' },
];

export default function HomeScreen() {
  const [status, setStatus] = useState<'esperando' | 'escaneando' | 'bloqueado' | 'votando'>('esperando');
  const [permission, requestPermission] = useCameraPermissions();
  const [tokenActivo, setTokenActivo] = useState<string | null>(null);

  // Función que se dispara al detectar el QR del ESP32
  const handleBarCodeScanned = ({ data }: { data: string }) => {
    // 'data' es el UUID que leyó del QR
    setTokenActivo(data);
    setStatus('votando'); 
    Alert.alert("Token Validado", "Ya puede efectuar su voto.");
  };

  const manejarVoto = async (nombreCandidato: string) => {
    Alert.alert(
      "Confirmación",
      `¿Desea confirmar su voto para: ${nombreCandidato}?`,
      [
        { text: "Cancelar", style: "cancel" },
        { 
          text: "Confirmar", 
          onPress: async () => {
            try {
              // ENVIAR VOTO AL BACKEND EN AWS
              const response = await fetch('http://44.196.23.48:3000/api/votar', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ 
                  token: tokenActivo, 
                  voto: nombreCandidato 
                }),
              });

              if (response.ok) {
                Alert.alert("Éxito", "Voto registrado correctamente.");
              } else {
                Alert.alert("Error", "Token inválido o ya utilizado.");
              }
            } catch (error) {
              Alert.alert("Error", "No se pudo conectar con el servidor.");
            }
            setStatus('esperando');
            setTokenActivo(null);
          } 
        }
      ]
    );
  };

  // Lógica de Permisos de Cámara
  if (status === 'escaneando') {
    if (!permission) return <View />;
    if (!permission.granted) {
      return (
        <View style={styles.container}>
          <Text style={styles.infoText}>Necesitamos permiso para usar la cámara</Text>
          <Button onPress={requestPermission} title="Otorgar Permiso" />
        </View>
      );
    }
    return (
      <View style={styles.container}>
        <Text style={styles.subTitulo}>Escanee el QR de la pantalla del dispositivo</Text>
        <CameraView
          onBarcodeScanned={handleBarCodeScanned}
          barcodeScannerSettings={{ barcodeTypes: ["qr"] }}
          style={styles.scanner}
        />
        <TouchableOpacity onPress={() => setStatus('esperando')} style={styles.btnCancelar}>
          <Text style={{color: 'white'}}>Cancelar</Text>
        </TouchableOpacity>
      </View>
    );
  }

  return (
    <View style={styles.container}>
      <Text style={styles.header}>SISTEMA ELECTORAL</Text>

      {/* 1. ESPERANDO */}
      {status === 'esperando' && (
        <View style={styles.card}>
          <Text style={styles.infoText}>Inicie el proceso escaneando el código de su puesto de votación.</Text>
          <TouchableOpacity 
            style={styles.botonIngresar} 
            onPress={() => setStatus('escaneando')}
          >
            <Text style={styles.botonTexto}>ESCANEAR QR DISPOSITIVO</Text>
          </TouchableOpacity>
        </View>
      )}

      {/* 2. BLOQUEADO (Igual que antes) */}
      {status === 'bloqueado' && (
        <View style={[styles.card, styles.cardError]}>
          <Text style={styles.errorTitle}>⚠️ ACCESO DENEGADO</Text>
          <Text style={styles.errorText}>DNI no habilitado para votar.</Text>
          <TouchableOpacity onPress={() => setStatus('esperando')} style={styles.btnVolver}>
              <Text>Volver</Text>
          </TouchableOpacity>
        </View>
      )}

      {/* 3. PANTALLA DE VOTACIÓN */}
      {status === 'votando' && (
        <ScrollView style={{ width: '100%' }}>
          <Text style={styles.subTitulo}>Token: {tokenActivo?.substring(0,8)}...</Text>
          {CANDIDATOS.map((c) => (
            <TouchableOpacity key={c.id} style={styles.cardCandidato} onPress={() => manejarVoto(c.nombre)}>
              <Text style={styles.nombreCandidato}>{c.nombre}</Text>
              <Text style={styles.partidoCandidato}>{c.partido}</Text>
            </TouchableOpacity>
          ))}
        </ScrollView>
      )}
    </View>
  );
}

const styles = StyleSheet.create({
  container: { flex: 1, backgroundColor: '#f0f4f7', alignItems: 'center', justifyContent: 'center', padding: 20, paddingTop: 60 },
  header: { fontSize: 26, fontWeight: 'bold', color: '#1a237e', marginBottom: 30 },
  subTitulo: { fontSize: 18, marginBottom: 20, textAlign: 'center', fontWeight: '600' },
  card: { backgroundColor: 'white', padding: 30, borderRadius: 15, width: '100%', alignItems: 'center', elevation: 4 },
  infoText: { textAlign: 'center', fontSize: 16, color: '#546e7a', marginBottom: 20 },
  cardError: { backgroundColor: '#ffebee', borderWidth: 2, borderColor: '#c62828' },
  errorTitle: { fontSize: 20, fontWeight: 'bold', color: '#c62828', marginBottom: 10 },
  errorText: { textAlign: 'center', color: '#b71c1c', marginBottom: 20 },
  botonIngresar: { backgroundColor: '#1a237e', padding: 20, borderRadius: 12, width: '100%' },
  botonTexto: { color: 'white', fontWeight: 'bold', textAlign: 'center', fontSize: 16 },
  cardCandidato: { backgroundColor: 'white', padding: 20, borderRadius: 10, marginBottom: 15, borderWidth: 1, borderColor: '#ddd' },
  nombreCandidato: { fontSize: 18, fontWeight: 'bold' },
  partidoCandidato: { color: '#666' },
  btnVolver: { marginTop: 10, padding: 10, backgroundColor: '#ddd', borderRadius: 5 },
  scanner: { width: '100%', height: 400, borderRadius: 20, overflow: 'hidden', marginBottom: 20 },
  btnCancelar: { backgroundColor: '#c62828', padding: 15, borderRadius: 10 }
});