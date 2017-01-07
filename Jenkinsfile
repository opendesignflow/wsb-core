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

  stage('Deploy') {
      sh "cd scala && ${mvnHome}/bin/mvn -B -Dmaven.test.failure.ignore deploy"
      step([$class: 'ArtifactArchiver', artifacts: '**/target/*.jar', fingerprint: true])
  }


}
