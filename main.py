from fastapi import FastAPI, Depends
from pydantic import BaseModel
from sqlalchemy import create_engine, Column, Integer, Float
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, Session

app = FastAPI()

# Configuración de la base de datos
SQLALCHEMY_DATABASE_URL = "sqlite:///./cars.db"  # Puedes cambiar el tipo de base de datos y la URL según tus necesidades

engine = create_engine(SQLALCHEMY_DATABASE_URL)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

Base = declarative_base()

# Modelo de la base de datos
class CarModel(Base):
    __tablename__ = "cars"

    id = Column(Integer, primary_key=True, index=True)
    velocidad = Column(Float)
    latitude = Column(Float)
    longitud = Column(Float)
    temperatura = Column(Float)
    humedad = Column(Float)
    altura = Column(Float)

Base.metadata.create_all(bind=engine)

# Dependencia para obtener una sesión de la base de datos
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

class Car(BaseModel):
    velocidad: float
    latitude: float
    longitud: float
    temperatura: float
    humedad: float
    altura: float

@app.get("/car")
def getData(db: Session = Depends(get_db)):
    cars = db.query(CarModel).all()
    return cars

@app.get("/car/latest")
def getLatestData(db: Session = Depends(get_db)):
    latest_car = db.query(CarModel).order_by(CarModel.id.desc()).first()
    return latest_car

@app.post("/car")
def postData(car: Car, db: Session = Depends(get_db)):
    db_car = CarModel(
        velocidad=car.velocidad,
        latitude=car.latitude,
        longitud=car.longitud,
        temperatura=car.temperatura,
        humedad=car.humedad,
        altura=car.altura
    )
    db.add(db_car)
    db.commit()
    db.refresh(db_car)
    return db_car


@app.get("/health")
def healt():
    return "."