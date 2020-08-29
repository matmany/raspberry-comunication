def tramaValues (trama = "000000000000"):
    idd = trama[0:4]
    s1 = trama[4:8]
    s2 = trama[8:12]
    data = {
        "id": idd,
        "s1": float(s1),
        "s2": float(s2)
    }
    return data;
    

