        // Select and construct the preconditioner
        autoPtr<lduMatrix::preconditioner> preconPtr =
        lduMatrix::preconditioner::New
        (
            *this,
            controlDict_
        );

        // Iterate
        do
        {
            // Store previous rho
            rhoOld = rho;

            // Apply precodnitioner to original and transposed systems
            preconPtr->precondition(w, r, cmpt);
            preconPtr->preconditionT(wT, rT, cmpt);

            if (0 == rhoOld)
            {
                // Attempt to restart
                rT = w;
                rho = gSumProd(w,rT);
            }
            else
            {
                // Update rho:
                rho = gSumProd(w, rT);   
	    }

            if (solverPerf.nIterations() == 0)
            {
                for (register label cell=0; cell<nCells; cell++)
                {
                    pPtr[cell] = wPtr[cell];
                    pTPtr[cell] = wTPtr[cell];
                }
            }
            else
            {
                scalar beta = rho/rhoOld;

                for (register label cell=0; cell<nCells; cell++)
                {
                    pPtr[cell] = wPtr[cell] + beta*pPtr[cell];
                    pTPtr[cell] = wTPtr[cell] + beta*pTPtr[cell];
                }
            }


            // Form Krylov subspace basis vectors
            matrix_.Amul(w, p, interfaceBouCoeffs_, interfaces_, cmpt);
            matrix_.Tmul(wT, pT, interfaceIntCoeffs_, interfaces_, cmpt);

            scalar wpT = gSumProd(w, pT);


            // Test for singularity
            if (solverPerf.checkSingularity(mag(wpT)/normFactor)) break;


            // Update solution and residual:
            scalar alpha = rho/wpT;

            for (register label cell=0; cell<nCells; cell++)
            {
                xPtr[cell] += alpha*pPtr[cell];
                rPtr[cell] -= alpha*wPtr[cell];
                rTPtr[cell] -= alpha*wTPtr[cell];
            }

            solverPerf.finalResidual() = gSumMag(r)/normFactor;

        } while
        (
            (++solverPerf.nIterations() < maxIter_
	     && 
             !(solverPerf.checkConvergence
               (
                   tolerance_, 
                   relTol_, 
                   solverPerf.nIterations(), 
                   minIter_
               )
              )
             )
        );
