#!/bin/bash
mkdir -v cocktail_contribs
root -b -x -q -l getCocktail.C\(200,\"rho\",\"cocktail_contribs/rho\",\"\"\)
for i in 19 27 39 62; do
  for p in pion eta etap rho omega phi jpsi; do
    root -l -b -x -q getCocktail.C\($i,\"$p\",\"cocktail_contribs/$p\",\"\"\)
  done
  root -l -b -x -q getCocktail.C\($i,\"hCocktail\",\"cocktail\",\"\"\)
  root -l -b -x -q getCocktail.C\($i,\"hCocktailPt_IMR\",\"cocktail\",\"_IMR_1.2-2.5\"\)
  root -l -b -x -q getCocktail.C\($i,\"hCocktailPt_jpsi\",\"cocktail\",\"_jpsi_2.97-3.22\"\)
  root -l -b -x -q getCocktail.C\($i,\"hCocktailPt_LMR\",\"cocktail\",\"_LMR_0.4-0.75\"\)
  root -l -b -x -q getCocktail.C\($i,\"hCocktailPt_omega\",\"cocktail\",\"_omega_0.76-0.82\"\)
  root -l -b -x -q getCocktail.C\($i,\"hCocktailPt_phi\",\"cocktail\",\"_phi_0.98-1.06\"\)
  root -l -b -x -q getCocktail.C\($i,\"hCocktailPt_pi0\",\"cocktail\",\"_pi0_0-0.2\"\)
  root -b -x -q -l getCocktail.C\($i,\"ccbar\",\"cocktail_contribs/ccbar\",\"\"\)
done
