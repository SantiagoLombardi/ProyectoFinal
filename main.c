#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PACIENTES 250
#define MAX_CONSULTAS 1000 // Ssponiendo que no haya mas de 1000 consultas en total

typedef struct
{
  int dia;
  int mes;
  int anio;
} TFecha;

typedef struct
{
  int DNI;
  char Apellido[50];
  char Nombres[50];
  TFecha FNacimiento;
  char Telefono[20];
  char Sexo;
} Paciente;

typedef struct
{
  int DNIPaciente;
  TFecha FConsulta;
  char Sintomas[100];
  char Tratamiento[100];
  char Control[20];
  char NombreMedico[50];
  char Especialidad[50];
} Consulta;

typedef struct NodoConsulta
{
  Consulta consulta;
  struct NodoConsulta *siguiente;
} NodoConsulta;

// Prototipos de funciones
void registrarPaciente(Paciente pacientes[], int *numPacientes);
void eliminarPaciente(Paciente pacientes[], int *numPacientes);
void registrarConsulta(Paciente pacientes[], int numPacientes, NodoConsulta **primerNodoConsulta, int *numConsultas);
void verHistoriaClinica(Paciente pacientes[], int numPacientes, NodoConsulta *primerNodoConsulta);
void consultasPorMedico(NodoConsulta *primerNodoConsulta, char nombreMedico[]);
void listadoChequeoPreventivo(Paciente pacientes[], int numPacientes);
TFecha obtenerFechaActual();
void cargarDatosDesdeArchivo(Paciente pacientes[], int *numPacientes, NodoConsulta **primerNodoConsulta);
void guardarDatosEnArchivo(Paciente pacientes[], int numPacientes, NodoConsulta *primerNodoConsulta);
int buscarPacientePorDNI(Paciente pacientes[], int numPacientes, int dni);
void mostrarPaciente(Paciente paciente);
void mostrarConsulta(Consulta consulta);
int compararFechas(TFecha fecha1, TFecha fecha2);

int main()
{
  Paciente pacientes[MAX_PACIENTES];
  NodoConsulta *primerNodoConsulta = NULL;
  int numPacientes = 0;
  int numConsultas = 0;

  // Carga los datos desde los archivos (si existen)
  cargarDatosDesdeArchivo(pacientes, &numPacientes, &primerNodoConsulta);

  int opcion;
  // Una vez que el programa tiene los datos, ejecuta en un bucle el "menu", ya que las funciones se llaman desde este evaluando con un switch la opción elegida
  // Cuando quien ejecuta el programa elige la opción 7 (Guardar y salir), el bucle termina y el programa finaliza
  do
  {
    printf("\nMenu de opciones:\n");
    printf("1) Registrar un nuevo paciente\n");
    printf("2) Eliminar a un paciente\n");
    printf("3) Registrar una consulta\n");
    printf("4) Ver Historia Clinica\n");
    printf("5) Ver consultas atendidas por un medico\n");
    printf("6) Listado Chequeo Preventivo\n");
    printf("7) Guardar y Salir\n");

    printf("Ingrese su opcion: ");
    scanf("%d", &opcion);

    // Switch que evalua la opcion elegida, luego llama a la función correspondiente
    switch (opcion)
    {
    case 1:
      registrarPaciente(pacientes, &numPacientes);
      break;
    case 2:
      eliminarPaciente(pacientes, &numPacientes);
      break;
    case 3:
      registrarConsulta(pacientes, numPacientes, &primerNodoConsulta, &numConsultas);
      break;
    case 4:
      verHistoriaClinica(pacientes, numPacientes, primerNodoConsulta);
      break;
    case 5:
    {
      char nombreMedico[50];
      system("cls");
      system("clear");
      printf("Ingrese el nombre del medico: ");
      
      getchar();
      fgets(nombreMedico, sizeof(nombreMedico), stdin);
      nombreMedico[strcspn(nombreMedico, "\n")] = 0;

      // scanf("%s", nombreMedico);
      consultasPorMedico(primerNodoConsulta, nombreMedico);
      break;
    }
    case 6:
      listadoChequeoPreventivo(pacientes, numPacientes);
      break;
    case 7:
      system("cls");
      system("clear");
      guardarDatosEnArchivo(pacientes, numPacientes, primerNodoConsulta);
      printf("Datos guardados. Saliendo del programa.\n");
      break;
    default:
      system("cls");
      system("clear");
      printf("Opcion invalida. Por favor, ingrese un numero valido.\n");
    }
  } while (opcion != 7);

  return 0;
}

void registrarPaciente(Paciente pacientes[], int *numPacientes)
{
  system("cls");
  system("clear");
  // Condición que evalúa la cantidad de pacientes con el máximo permitido
  if (*numPacientes >= MAX_PACIENTES)
  {
    printf("Se ha alcanzado el limite maximo de pacientes. No se puede agregar mas.\n");
    return;
  }

  // si el condicional anterior tuvo un valor falso, se crea un paciente nuevo con la estructura Paciente
  Paciente nuevoPaciente;
  printf("Ingrese el DNI del paciente: ");
  scanf("%d", &nuevoPaciente.DNI);

  // Condición que evalúa si ya existe algún paciente con el DNI ingresado
  if (buscarPacientePorDNI(pacientes, *numPacientes, nuevoPaciente.DNI) != -1)
  {
    printf("Ya existe un paciente con ese DNI. No se puede agregar.\n");
    return;
  }

  printf("Ingrese el Apellido del paciente: ");
  getchar(); 
  fgets(nuevoPaciente.Apellido, sizeof(nuevoPaciente.Apellido), stdin);
  nuevoPaciente.Apellido[strcspn(nuevoPaciente.Apellido, "\n")] = 0;

  printf("Ingrese el Nombre del paciente: ");

  fgets(nuevoPaciente.Nombres, sizeof(nuevoPaciente.Nombres), stdin);
  nuevoPaciente.Nombres[strcspn(nuevoPaciente.Nombres, "\n")] = 0;

  printf("Ingrese la fecha de nacimiento del paciente (DD MM AAAA): ");
  scanf("%d %d %d", &nuevoPaciente.FNacimiento.dia, &nuevoPaciente.FNacimiento.mes, &nuevoPaciente.FNacimiento.anio);

  printf("Ingrese el telefono del paciente: ");
  scanf("%s", nuevoPaciente.Telefono);

  printf("Ingrese el sexo del paciente (M/F/X): ");
  scanf(" %c", &nuevoPaciente.Sexo);

  // Una vez se crea un nuevo paciente se actualiza el número total de pacientes
  pacientes[*numPacientes] = nuevoPaciente;
  (*numPacientes)++;

  printf("Paciente agregado correctamente.\n");
}

void eliminarPaciente(Paciente pacientes[], int *numPacientes)
{
  system("cls");
  system("clear");
  int dni;
  printf("Ingrese el DNI del paciente que desea eliminar: ");
  scanf("%d", &dni);

  // se busca al paciente que se quiere eliminar entre los existentes para poder eliminarlo o en caso de que no exista evaluar esto y devolver el mensaje correspondiente
  int indicePaciente = buscarPacientePorDNI(pacientes, *numPacientes, dni);

  // condicional que evalua si el usuario que se quiere eliminar existe, en caso de no existir (cuando la condicion evaluada sea verdadera) no se elimina ningun paciente y se vuelve al menú
  if (indicePaciente == -1)
  {
    printf("El DNI ingresado no pertenece a ningun paciente registrado.\n");
    return;
  }

  // si el paciente existe, se elimina moviendo los elementos posteriores
  pacientes[indicePaciente] = pacientes[*numPacientes - 1];
  (*numPacientes)--;

  printf("Paciente eliminado correctamente.\n");
}

void registrarConsulta(Paciente pacientes[], int numPacientes, NodoConsulta **primerNodoConsulta, int *numConsultas)
{
  system("cls");
  system("clear");

  // condicional que evalua si se alcanzo el maximo permitido de consultas, en caso de ser asi (condicional verdadero) no se permite agregar otra consulta y se vuelve al menú
  if (*numConsultas >= MAX_CONSULTAS)
  {
    printf("Se ha alcanzado el limite maximo de consultas. No se puede registrar mas.\n");
    return;
  }

  int dni;
  printf("Ingrese el DNI del paciente: ");
  scanf("%d", &dni);

  // antes de registrar la busqueda se evalua si existe el paciente ya existe en el registro
  // se necesita que el paciente exista en el registro para poder registrar la consulta
  int indicePaciente = buscarPacientePorDNI(pacientes, numPacientes, dni);
  if (indicePaciente == -1)
  {
    printf("El DNI ingresado no pertenece a ningun paciente registrado, registre el paciente primero y luego intente nuevamente.\n");
    return;
  }

  // si el paciente si existe (osea el condicional anterior evaluo la condicion como falsa) se crea la consulta
  Consulta nuevaConsulta;
  nuevaConsulta.DNIPaciente = dni;
  // se registra la fecha de la consulta, usando la fecha del sistema
  nuevaConsulta.FConsulta = obtenerFechaActual();

  printf("Ingrese los sintomas: ");
  getchar();
  fgets(nuevaConsulta.Sintomas, sizeof(nuevaConsulta.Sintomas), stdin);
  nuevaConsulta.Sintomas[strcspn(nuevaConsulta.Sintomas, "\n")] = 0;

  printf("Ingrese el tratamiento: ");
  
  fgets(nuevaConsulta.Tratamiento, sizeof(nuevaConsulta.Tratamiento), stdin);
  nuevaConsulta.Tratamiento[strcspn(nuevaConsulta.Tratamiento, "\n")] = 0;


  // ya que en el pdf las opciones ya estaban definidas y no habia una opcion como "otros", hice que el usuario tenga que elegir entre las opciones con un switch en vez de ingresar "manualmente" cada cuanto deberia ser el control
  printf("Ingrese el control (Seleccione una opcion):\n");
  printf("1) No\n");
  printf("2) 24hs\n");
  printf("3) 48hs\n");
  printf("4) 72hs\n");
  printf("5) 1 Semana\n");
  printf("6) 1 Mes\n");

  int opcionControl;
  scanf("%d", &opcionControl);
  switch (opcionControl)
  {
  case 1:
    strcpy(nuevaConsulta.Control, "No");
    break;
  case 2:
    strcpy(nuevaConsulta.Control, "24hs");
    break;
  case 3:
    strcpy(nuevaConsulta.Control, "48hs");
    break;
  case 4:
    strcpy(nuevaConsulta.Control, "72hs");
    break;
  case 5:
    strcpy(nuevaConsulta.Control, "1 Semana");
    break;
  case 6:
    strcpy(nuevaConsulta.Control, "1 Mes");
    break;
  // si la entrada no se corresponde con las opciones existentes por defecto se el "Control" de la consulta se define como "No"
  default:
    printf("Opcion no valida. Se registrara como \"No corresponde\".\n");
    strcpy(nuevaConsulta.Control, "No");
  }

  printf("Ingrese el nombre del medico: ");
  getchar();
  fgets(nuevaConsulta.NombreMedico, sizeof(nuevaConsulta.NombreMedico), stdin);
  nuevaConsulta.NombreMedico[strcspn(nuevaConsulta.NombreMedico, "\n")] = 0;

  printf("Ingrese la especialidad del medico: ");
  
  fgets(nuevaConsulta.Especialidad, sizeof(nuevaConsulta.Especialidad), stdin);
  nuevaConsulta.Especialidad[strcspn(nuevaConsulta.Especialidad, "\n")] = 0;

  // se crea un nuevo nodo para la consulta
  NodoConsulta *nuevoNodo = (NodoConsulta *)malloc(sizeof(NodoConsulta));
  nuevoNodo->consulta = nuevaConsulta;
  nuevoNodo->siguiente = NULL;

  // se inserta el nodo en la lista enlazada en orden
  if (*primerNodoConsulta == NULL || (*primerNodoConsulta)->consulta.DNIPaciente > nuevoNodo->consulta.DNIPaciente ||
      ((*primerNodoConsulta)->consulta.DNIPaciente == nuevoNodo->consulta.DNIPaciente &&
       compararFechas((*primerNodoConsulta)->consulta.FConsulta, nuevoNodo->consulta.FConsulta) > 0))
  {
    nuevoNodo->siguiente = *primerNodoConsulta;
    *primerNodoConsulta = nuevoNodo;
  }
  else
  {
    NodoConsulta *actual = *primerNodoConsulta;
    while (actual->siguiente != NULL &&
           (actual->siguiente->consulta.DNIPaciente < nuevoNodo->consulta.DNIPaciente ||
            (actual->siguiente->consulta.DNIPaciente == nuevoNodo->consulta.DNIPaciente &&
             compararFechas(actual->siguiente->consulta.FConsulta, nuevoNodo->consulta.FConsulta) < 0)))
    {
      actual = actual->siguiente;
    }
    nuevoNodo->siguiente = actual->siguiente;
    actual->siguiente = nuevoNodo;
  }

  // se actualiza el numero de consultas existentes
  (*numConsultas)++;
  printf("Consulta registrada correctamente.\n");
}

void verHistoriaClinica(Paciente pacientes[], int numPacientes, NodoConsulta *primerNodoConsulta)
{
  system("cls");
  system("clear");
  int dni;
  printf("Ingrese el DNI del paciente: ");
  scanf("%d", &dni);

  // se busca el paciente entre los existentes, luego se evalua si existe o no
  int indicePaciente = buscarPacientePorDNI(pacientes, numPacientes, dni);

  // en caso de no existir se imprime un mensaje correspondiente a ese contexto
  if (indicePaciente == -1)
  {
    printf("El DNI ingresado no pertenece a ningun paciente registrado.\n");
    return;
  }

  // en caso de existir se imprimen el paciente y sus consultas cronologicamente
  printf("Historia Clinica del Paciente:\n");
  mostrarPaciente(pacientes[indicePaciente]);
  printf("Consultas:\n");
  printf("-------------------------------\n");

  // se recorre la LSE y se obtienen las consultas que corresponden al paciente ingresado, comparando el DNI ingresado y el existente en el nodo
  NodoConsulta *actual = primerNodoConsulta;
  while (actual != NULL)
  {
    if (actual->consulta.DNIPaciente == dni)
    {
      mostrarConsulta(actual->consulta);
      printf("-------------------------------\n");
    }
    actual = actual->siguiente;
  }
}

void consultasPorMedico(NodoConsulta *primerNodoConsulta, char nombreMedico[])
{
  printf("Consultas atendidas por el medico %s:\n", nombreMedico);
  printf("-------------------------------\n");
  int encontradas = 0;

  // recorre las consultas existentes y compara la constante "NombreMedico" de las consultas existentes con la variable "nombreMedico" de la funcion, ademas cuando encuentra alguna consulta que corresponda actualiza el valor de la variable "encontradas" importante para un condicional dentro de la funcion
  NodoConsulta *actual = primerNodoConsulta;
  while (actual != NULL)
  {
    if (strcmp(actual->consulta.NombreMedico, nombreMedico) == 0)
    {
      mostrarConsulta(actual->consulta);
      printf("-------------------------------\n");
      encontradas++;
    }
    actual = actual->siguiente;
  }

  // si no se encontro ninguna consulta que corresponda a un medico con el nombre ingresado, se imprime un mensaje expresando esto
  if (encontradas == 0)
  {
    printf("No se encontraron consultas atendidas por el medico %s.\n", nombreMedico);
  }
}

void listadoChequeoPreventivo(Paciente pacientes[], int numPacientes)
{
  system("cls");
  system("clear");
  printf("Listado de Chequeo Preventivo (pacientes mayores de 40):\n");

  TFecha fechaActual = obtenerFechaActual();

  // valor importante para evaluar si se encontaron pacientes mayores de 40 o no, aporta contexto a la funcion
  int encontrado = 0;

  // el bucle recorre los pacientes comparando la diferencia entre el año actual obtenido desde el sistema y el año de nacimiento, con 40, obteniendo los pacientes que tienen 40 o mas años de edad
  // Nota: esta forma de evaluar la edad no es precisa porque evalua solamente por el año, se podria optar por ser mas precisos aunque de  igual manera el margen de error es de un año menos un dia, por lo que no es tan relevante (a mi criterio)
  for (int i = 0; i < numPacientes; ++i)
  {
    int edad = fechaActual.anio - pacientes[i].FNacimiento.anio;
    if (edad >= 40)
    {
      // una vez encontrados los pacientes deseados se imprime su nombre y telefono
      printf("Nombre: %s %s, Telefono: %s\n", pacientes[i].Nombres, pacientes[i].Apellido, pacientes[i].Telefono);
      encontrado++;
    }
  }

  // si no se encontraron pacientes mayores de 40 años, se imprime un mensaje comunicandolo
  if (!encontrado)
  {
    printf("No hay pacientes mayores de 40 registrados.\n");
  }
}

TFecha obtenerFechaActual()
{
  // la implementacion de esta funcion para obtener la fecha esta basada en la explicacion de la pagina de techie delight, usando la libreria time.h
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  TFecha fechaActual;
  fechaActual.dia = tm.tm_mday;
  fechaActual.mes = tm.tm_mon + 1;      // se suma 1, porque los meses estan enumerados de 0 a 11
  fechaActual.anio = tm.tm_year + 1900; // se suma 1900, porque los años se cuentan desde 1900

  return fechaActual;
}

void cargarDatosDesdeArchivo(Paciente pacientes[], int *numPacientes, NodoConsulta **primerNodoConsulta)
{
  // se definen los punteros de los archivos y abre los archivos para lectura de binarios
  FILE *archivoPacientes = fopen("pacientes.dat", "rb");
  FILE *archivoConsultas = fopen("consultas.dat", "rb");

  // el condicional verifica que existan los archivos, para luego leer y almacenar "localmente" los datos de los archivos .dat
  if (archivoPacientes != NULL && archivoConsultas != NULL)
  {
    fread(numPacientes, sizeof(int), 1, archivoPacientes);
    fread(pacientes, sizeof(Paciente), *numPacientes, archivoPacientes);

    while (!feof(archivoConsultas))
    {
      Consulta nuevaConsulta;
      if (fread(&nuevaConsulta, sizeof(Consulta), 1, archivoConsultas) != 1)
      {
        break;
      }
      NodoConsulta *nuevoNodo = (NodoConsulta *)malloc(sizeof(NodoConsulta));
      nuevoNodo->consulta = nuevaConsulta;
      nuevoNodo->siguiente = NULL;
      if (*primerNodoConsulta == NULL)
      {
        *primerNodoConsulta = nuevoNodo;
      }
      else
      {
        NodoConsulta *temp = *primerNodoConsulta;
        while (temp->siguiente != NULL)
        {
          temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
      }
    }

    fclose(archivoPacientes);
    fclose(archivoConsultas);

    printf("Datos cargados desde los archivos.\n");
  }
  else
  {
    printf("No se encontraron archivos de datos existentes. Iniciando con datos vacios.\n");
  }
}

void guardarDatosEnArchivo(Paciente pacientes[], int numPacientes, NodoConsulta *primerNodoConsulta)
{
  // se definen los punteros de los archivos y abre los archivos para escritura de binarios
  FILE *archivoPacientes = fopen("pacientes.dat", "wb");
  FILE *archivoConsultas = fopen("consultas.dat", "wb");

  if (archivoPacientes != NULL && archivoConsultas != NULL)
  {
    fwrite(&numPacientes, sizeof(int), 1, archivoPacientes);
    fwrite(pacientes, sizeof(Paciente), numPacientes, archivoPacientes);

    NodoConsulta *actual = primerNodoConsulta;
    while (actual != NULL)
    {
      fwrite(&(actual->consulta), sizeof(Consulta), 1, archivoConsultas);
      actual = actual->siguiente;
    }

    fclose(archivoPacientes);
    fclose(archivoConsultas);

    printf("Datos guardados en los archivos.\n");
  }
  else
  {
    printf("No se pudo guardar los datos en los archivos.\n");
  }
}

int buscarPacientePorDNI(Paciente pacientes[], int numPacientes, int dni)
{
  // esta funcion busca un paciente por su DNI y devuelve su índice en el arreglo de pacientes
  for (int i = 0; i < numPacientes; i++)
  {
    if (pacientes[i].DNI == dni)
    {
      return i; // se encontró el paciente en la posición i
    }
  }
  return -1; // no se encontró el paciente
}

void mostrarPaciente(Paciente paciente)
{
  printf("DNI: %d\n", paciente.DNI);
  printf("Apellido: %s\n", paciente.Apellido);
  printf("Nombres: %s\n", paciente.Nombres);
  printf("Fecha de Nacimiento: %02d/%02d/%d\n", paciente.FNacimiento.dia, paciente.FNacimiento.mes, paciente.FNacimiento.anio);
  printf("Telefono: %s\n", paciente.Telefono);
  printf("Sexo: %c\n", paciente.Sexo);
}

void mostrarConsulta(Consulta consulta)
{
  printf("Fecha de Consulta: %02d/%02d/%d\n", consulta.FConsulta.dia, consulta.FConsulta.mes, consulta.FConsulta.anio);
  printf("Sintomas: %s\n", consulta.Sintomas);
  printf("Tratamiento: %s\n", consulta.Tratamiento);
  printf("Control: %s\n", consulta.Control);
  printf("Nombre del Medico: %s\n", consulta.NombreMedico);
  printf("Especialidad del Medico: %s\n", consulta.Especialidad);
}

int compararFechas(TFecha fecha1, TFecha fecha2)
{
  // esta funcion compara dos fechas, devolviendo -1 si fecha1 es anterior a fecha2, 1 si es posterior y 0 si son iguales
  if (fecha1.anio < fecha2.anio)
  {
    return -1;
  }
  else if (fecha1.anio > fecha2.anio)
  {
    return 1;
  }
  else
  {
    if (fecha1.mes < fecha2.mes)
    {
      return -1;
    }
    else if (fecha1.mes > fecha2.mes)
    {
      return 1;
    }
    else
    {
      if (fecha1.dia < fecha2.dia)
      {
        return -1;
      }
      else if (fecha1.dia > fecha2.dia)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
  }
}
