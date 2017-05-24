

// Start Build Step function
def transformIntoStep(jobFullName) {
    return {
       build job: jobFullName , wait: false, propagate: false
    }
}

// WSB Core
node {
 
  properties([pipelineTriggers([[$class: 'GitHubPushTrigger']])])
  def mvnHome = tool 'maven3'

  stage('Clean') {
    checkout scm
    sh "cd scala && ${mvnHome}/bin/mvn -B clean"
  }

  stage('Build') {
    sh "cd scala && ${mvnHome}/bin/mvn -B  compile test-compile"
  }

  stage('Test') {
    sh "cd scala && ${mvnHome}/bin/mvn -B -Dmaven.test.failure.ignore test"
//    junit '**/target/surefire-reports/TEST-*.xml'
  }


  // Deploy Or Package
  //-----------------
  if (env.BRANCH_NAME == 'dev' || env.BRANCH_NAME == 'master') {
    
    stage('Deploy') {
      sh "cd scala && ${mvnHome}/bin/mvn -B -Dmaven.test.failure.ignore deploy"
      step([$class: 'ArtifactArchiver', artifacts: '**/target/*.jar', fingerprint: true])
    }

    // Trigger sub builds on dev
    if (env.BRANCH_NAME == 'dev') {
      stage('Downstream') {

        def downstreams = ['../wsb-webapp/dev',]
        def stepsForParallel = [:]
        for (x in downstreams) {
          def ds = x 
          stepsForParallel[ds] = transformIntoStep(ds) 
        }
      
        parallel stepsForParallel

      }

    }
    // EOF Downstream

  } else {
    
    stage('Package') {
        sh "cd scala && ${mvnHome}/bin/mvn -B -Dmaven.test.failure.ignore package"
        step([$class: 'ArtifactArchiver', artifacts: '**/target/*.jar', fingerprint: true])
    }
  
  }

}
