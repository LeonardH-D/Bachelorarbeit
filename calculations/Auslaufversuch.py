from scipy.integrate import solve_ivp
import matplotlib.pyplot as plt
import numpy as np
import math

r = 0.03 #m
N = 12
m = 0.0115 #kg, pro Lager
J = r**2 * m * N

wrotN = 8600/60 * 2*math.pi
ErotN = 0.5* wrotN**2 * J

b_v = 0.00001  #Nm/ (rad/s)  #Viskose Reibung
F_v = 0        #Nm           #Trockene Reibung
c_v = 0.2e-8     #Nm/ (rad/s)^2 #Ventilation

tau = J / b_v 

def ode_auslauf(t, statevars):
    phi = statevars[0]
    w = statevars[1]
    
       
    #calculation of derivatives    
    dphi_dt = w
    M_verlust = b_v*w + (w>0)*c_v* w**2 + F_v
    dw_dt = 1/J * -M_verlust
    
    der_statevars = [dphi_dt, dw_dt]

    return der_statevars 


#add IVP / simulation parameters
T_max = 50.0
#T_max = 0.1
Ts = 100e-3 
phi0 = 0.0
w0 = wrotN

sol = solve_ivp(ode_auslauf, [0,T_max], [phi0, w0], method='RK45', max_step=Ts)
phi = sol.y[0]
w = sol.y[1]
t = sol.t

fig, (ax_w, ax_P, ax_E) = plt.subplots(3, 1)

ax_w.plot(t, w/2/math.pi*60, 'b-')
ax_w.grid(1)
ax_w.set_ylabel(r"$n/ \mathrm{min^{-1}}$")

M_verlust = b_v*w + (w>0)*c_v* w**2 + F_v
P_verlust = M_verlust*w
ax_P.plot(t, P_verlust, 'b-')
ax_P.grid(1)
ax_P.set_ylabel(r"$P/ \mathrm{W}$")

E = 0.5*J*w**2
ax_E.plot(t, E, 'b-')
ax_E.grid(1)
ax_E.set_ylabel(r"$E/ \mathrm{Ws}$")
ax_E.set_xlabel(r"$t/ \mathrm{s}$")


#plt.savefig("cart_python.png", dpi=300)

