        // Select and construct the preconditioner
        autoPtr<lduMatrix::preconditioner> preconPtr =
        lduMatrix::preconditioner::New
        (
            *this,
            controlDict_
        );

        // Solver iteration
        do
        {
            // Store previous rho
            rhoOld = rho;

            // Apply preconditioner
            preconPtr->precondition(w, r, cmpt);

            // Update rho:
            rho = gSumProd(w, r);

            if (solverPerf.nIterations() == 0)
            {
                for (register label cell=0; cell<nCells; ++cell)
                {
                    pPtr[cell] = wPtr[cell];
                }
            }
            else
            {
                scalar beta = rho/rhoOld;

                for (register label cell=0; cell<nCells; ++cell)
                {
                    pPtr[cell] = wPtr[cell] + beta*pPtr[cell];
                }
            }


            // Update preconditioned residual
            matrix_.Amul(w, p, interfaceBouCoeffs_, interfaces_, cmpt);

            scalar wApA = gSumProd(w, p);


            // Test for singularity
            if (solverPerf.checkSingularity(mag(wApA)/normFactor)) 
                break;

            // Update solution and residual:
            scalar alpha = rho/wApA;

            for (register label cell=0; cell<nCells; ++cell)
            {
                xPtr[cell] += alpha*pPtr[cell];
                rPtr[cell] -= alpha*wPtr[cell];
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
