import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

seed=1
np.random.seed(seed)
SDF=0.6
sigma = np.sqrt(-np.log(SDF))
mean = 1.58E7
#normal_std = np.sqrt(np.log(1 + (sigma/mean)**2))
normal_mean = np.log(mean) - sigma**2 / 2
n_events = 100000000
print("SDF = ", SDF)
print("Mean POT = ",mean)
rng = np.random.default_rng()
POTs = rng.lognormal(normal_mean, sigma, n_events)
#print(POTs[:10])

d = {'N_POT' : POTs}
df = pd.DataFrame(data=d)

mu_stop_per_POT = 0.0015
df["N_MuStop"] = df["N_POT"] * mu_stop_per_POT

f_cap = 0.609
df["N_MuCap"] = df["N_MuStop"] * f_cap

true_ce_rate = 1e-10
df["Expected_CEs"] = df["N_MuCap"] * true_ce_rate

df["N_CEs"] = np.random.poisson(df["Expected_CEs"])

#print(df.head())
#print(df.sum())
total_POT = df["N_POT"].sum()
print("Total POT = {:.3E}".format(total_POT))
total_mu_caps = df["N_MuCap"].sum()
measured_ces = df["N_CEs"].sum()
print("Measured CEs = ", measured_ces)
print("Total Mu Caps = {:.2E}".format(total_mu_caps))
measured_ce_rate = measured_ces / total_mu_caps
print("Ce Rate = ", true_ce_rate)
print("Measured Ce Rate = {:.2E}".format(measured_ce_rate))

df_trig = df[df["N_CEs"]>0]
print("Assuming perfect trigger that only triggers on Ce events");
print(df_trig.head())

POT_cut = 1e7;
df_unveto = df_trig[df_trig["N_POT"]<POT_cut]
df_veto = df_trig[df_trig["N_POT"]>=POT_cut]
print("Assuming analysis veto of whole event when N_POT>{:.2E}".format(POT_cut))
unvetoed_ces = df_unveto["N_CEs"].sum()
unvetoed_ce_rate = unvetoed_ces / total_mu_caps;
print("Unvetoed CEs = ", unvetoed_ces)
print("Unvetoed Ce Rate = {:.2E}".format(unvetoed_ce_rate))

vetoed_mu_caps = df_veto["N_MuCap"].sum()
corrected_ce_rate = unvetoed_ces / (total_mu_caps - vetoed_mu_caps);
print("Mu Caps from Vetoed Events = {:.2E}".format(vetoed_mu_caps))
print("Corrected Ce Rate = {:.2E}".format(corrected_ce_rate))

fig, ax = plt.subplots()
_, bins, _ = ax.hist(df_trig["N_POT"], bins=100, align='mid')
ax.hist(df_unveto["N_POT"], bins=bins, align='mid', histtype='step')
plt.show()

# total_mu_stops = sum(mu_stops)
# total_mu_caps = f_cap * total_mu_stops
# print ("N Generated Events = {:.0E}".format(n_events))
# print ("Total POT = {:.3E}".format(total_POTs))
# print ("Total Mu Stops = {:.3E}".format(total_mu_stops))
# print ("Total Mu Caps = {:.3E}".format(total_mu_caps))

# ce_rate = 1e-11
# n_ces = np.random.poisson(mu_stops * f_cap * ce_rate)
# print("Measured N Ces = ", n_ces[:10]);
# total_ces = sum(n_ces)
# total_ces_err = np.sqrt(total_ces)
# print ("Total Ces = ", total_ces, " +/- {:.1E}".format(total_ces_err));
# print ("True Ce Rate = ", ce_rate)
# print ("Measured Ce Rate = {:.3E} +/- {:.3E}".format(total_ces / total_mu_caps, total_ces_err / total_mu_caps))

# measured_events = n_ces > 0
# print(measured_events)
# #veto_probs = POTs / max(POTs)
# #print (veto_probs[:10])
