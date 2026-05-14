import { CameraView, useCameraPermissions } from 'expo-camera';
import React, { useState } from 'react';
import { Alert, Button, StyleSheet, Text, View } from 'react-native';

export default function ScannerScreen() {
  const [permission, requestPermission] = useCameraPermissions();
  const [scanned, setScanned] = useState(false);

  if (!permission) return <View />;
  if (!permission.granted) {
    return (
      <View style={styles.container}>
        <Text>Necesitamos permiso para usar la cámara</Text>
        <Button onPress={requestPermission} title="Otorgar permiso" />
      </View>
    );
  }

  const handleBarCodeScanned = async ({ data }: { data: string }) => {
    setScanned(true);
    
    // 'data' contiene el UUID que recibió el ESP32 por MQTT
    try {
      const response = await fetch('http://44.196.23.48:3000/api/votar', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ 
          token: data, 
          candidatoId: "Candidato_Seleccionado" 
        }),
      });

      if (response.ok) {
        Alert.alert("Éxito", "Voto registrado correctamente");
      } else {
        Alert.alert("Error", "El token no es válido o ya fue usado");
      }
    } catch (error) {
      Alert.alert("Error de red", "No se pudo conectar con el servidor en AWS");
    }
  };

  return (
    <View style={styles.container}>
      <CameraView
        onBarcodeScanned={scanned ? undefined : handleBarCodeScanned}
        barcodeScannerSettings={{ barcodeTypes: ["qr"] }}
        style={StyleSheet.absoluteFillObject}
      />
      {scanned && (
        <Button title={'Escanear de nuevo'} onPress={() => setScanned(false)} />
      )}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    flexDirection: 'column',
    justifyContent: 'center',
  },
});